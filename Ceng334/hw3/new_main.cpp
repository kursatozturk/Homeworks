#include "main.hpp"

using namespace std;

template<class T>
CAST(T)* findFreeDataBlock (uint *id = nullptr) {
    typedef CAST(T) DataBlock;
    GroupDesc *gd = new GroupDesc(desc);
    // bitmap holder
    BitMap *blockBitMap = new BitMap(0, BitMapSize);
    size_t blocks_per_group = GET(superBlock, s_blocks_per_group);
    for (size_t group = 0; group < num_of_block_groups; ++group) {
        gd->move(group);
        blockBitMap->setOffset(GET(gd, bg_block_bitmap));
        blockBitMap->get(GET(superBlock, s_blocks_per_group));
        // find the idx of first data_block of group
        uint data_block = GET(gd, bg_inode_table) + 
                            ceil(GET(superBlock, s_inodes_per_group) * INodeSize/block_size);

        uint group_offset = group * GET(superBlock, s_blocks_per_group);
        uint blockmap_offset = data_block - group_offset;
        for (size_t i = blockmap_offset; i < blocks_per_group; ++i) {
            if (FREE(i, blockBitMap)) {
                cout << "i found: " << i << endl;
                if (id) *id = i + group_offset + 1;
                // mark bitmap to state block is reserved.
                RESERVE(i, blockBitMap);
                blockBitMap->save();
                GET(superBlock, s_free_blocks_count)--;
                GET(superBlock, s_r_blocks_count)++;
                superBlock->save();
                GET(gd, bg_free_blocks_count)--;
                gd->save();
                //update desc
                desc->get();
                // 
                DataBlock* d = new DataBlock(i + group_offset + 1, sizeof(T));
                delete gd;
                delete blockBitMap;
                cout << "returning from data block founder " << endl;
                return d;
            }
        }
    }
}

INode *findFreeINodeBlock (uint *id = nullptr) {
    GroupDesc *gd = new GroupDesc(desc);
    // bitmap holder
    BitMap *inodeBitMap = new BitMap(0, BitMapSize);
    size_t inodes_per_group = GET(superBlock, s_inodes_per_group);
    for (size_t group = 0; group < num_of_block_groups; ++group) {
        gd->move(group);
        inodeBitMap->setOffset(GET(gd, bg_inode_bitmap));
        for (size_t i = 0; i < inodes_per_group; ++i) {
            if (FREE(i, inodeBitMap)) {
                if (id) *id = i + 1;
                RESERVE(i, inodeBitMap);
                inodeBitMap->save();
                INode *availINode = new INode(GET(gd, bg_inode_table), INodeSize);
                availINode->move(i);
                GET(superBlock, s_free_inodes_count)--;
                superBlock->save();
                GET(gd, bg_free_inodes_count)--;
                gd->save();
                //update desc
                desc->get();
                delete gd;
                delete inodeBitMap;
                return availINode;
            }
        }
    }
    return nullptr;
}
    
int indirect_link2Inode(Block* link_block, const uint& id, unsigned char depth) {
    /*
        block_ids: Holds link ids. If depth == 1, links are direct(namely, directly points data),
                else(depth > 1) links are indirect.        
        id: index of the free data block found. It will be put to first appropriate place.
        depth: holds the depth of indirect links. i.e. : 
                        if is a single indirect link, depth==1
                        if is a doubly indirect link, depth==2
    */
   uint *block_ids = link_block->cast2type<uint>();
   if (depth == 1) {
       for (uint i = 0; i < block_size / sizeof(uint); ++i) {
           if (!block_ids[i]) {
               block_ids[i] = id;
               link_block->save();
               return i;
           }
       }
   }
   else {
       for (uint i = 0; i < block_size / sizeof(uint); ++i) {
           Block* indirect_block_ids = nullptr;
           if (!block_ids[i]) {
                // if no block is reserved,
                // reserve a block for indirect pointer.
                indirect_block_ids =  findFreeDataBlock<char>(&block_ids[i]);

           }
           else {
               indirect_block_ids = new Block (block_ids[i]);
           }
           // read whole block
           indirect_block_ids->get(block_size);
           auto idx = indirect_link2Inode(indirect_block_ids, id, depth - 1);
       }
   }
}

void link_block2Inode(INode* fInode, const uint &id) {
    /*
        fInode: INode which required a free data block.
        id: index of the free data block found. It will be put to first appropriate place.
        dblock_id: Holds the id of the free block found.
        depth: holds the depth of indirect links. i.e. : 
                        if is a single indirect link, depth==1
                        if is a doubly indirect link, depth==2
    */
    int dblock_id = -1;
    for (char i = 0; i < 12; ++i) {
        if (!GET(fInode, i_block)[i]) {
            dblock_id = i;
            GET(fInode, i_block)[i] = id;
            break;
        }
    }
    if (dblock_id == -1) {
        Block* indirect_block_ids = nullptr;
        int depth = 1;
        do {
            if (!GET(fInode, i_block[11+depth])) {
                // if no block is reserved for indirect links,
                // reserve a block.
                indirect_block_ids = findFreeDataBlock<char>(&GET(fInode, i_block[11 + depth]));
                GET(fInode, i_blocks) += block_size/512;
                cerr << "what is this?" << endl;
            }
            else {
                indirect_block_ids = new Block(GET(fInode, i_block[11 + depth]));
            }
            
            // read whole block
            indirect_block_ids->get(block_size);

            dblock_id = indirect_link2Inode(indirect_block_ids, id, depth);
            depth++;
            delete indirect_block_ids;
        }while (dblock_id == -1 && depth <= 3);
    }
    fInode->save();
}

void putFile2DataBlock(const string& fn, INode* fInode) {
    long fs;
    char *file_content = readContent(fn, &fs);

    char *d = file_content;
    do {
        uint d_id;
        Block* dataBlock = findFreeDataBlock<char>(&d_id);

        // put pointer(id) to i_block of fINode
        link_block2Inode(fInode, d_id);

        // determine how many byte will be written to block
        auto write_byte = min((uint)fs, block_size);
        fs -= block_size;
        
        GET(fInode, i_blocks)+= block_size/512; // not sure about 512
       
        // copy file to dataBlock
        dataBlock->blockWrite(write_byte, d);
        d += write_byte;
        delete dataBlock;
    }while(fs > 0);
    delete[] file_content;
}

bool fillDirectoryEntry (
    Block* &availBlock, // DirEntry to be filled
    const string& fn, // name of the file

    uint& inodeNo    // no of inode block
                    // where inode of file resides.
    ) {
    typedef struct ext2_dir_entry dir_entry;
    size_t dir_length = sizeof(int) + sizeof(short) + 2 * sizeof(char) + fn.length();
    availBlock->cast2type<dir_entry>()->name_len = fn.length();
    FillStr(availBlock->cast2type<dir_entry>()->name, fn);  
    availBlock->cast2type<dir_entry>()->inode = inodeNo;

    auto buf = getStat(fn);
    
    availBlock->cast2type<dir_entry>()->file_type = S_ISREG(buf->st_mode)? EXT2_FT_REG_FILE: EXT2_FT_DIR;

    return true;
}

bool checkDirEntry(uint &dataBlockId, size_t rec_len, Block* &avail_dir_entry) {
    typedef struct ext2_dir_entry dir_entry;

    Block* dataBlock = nullptr;
    if (dataBlockId == 0) {
        // find a free DataBlock.
        dataBlock = findFreeDataBlock<char> (&dataBlockId);
    }
    else {
        dataBlock = new Block(dataBlockId);
    }
    // read dirEntry Block
    dataBlock->get(block_size);
    uint checked_mem = 0;
    auto entry_list = dataBlock->cast2type<dir_entry>();
    while (checked_mem + entry_list->rec_len < block_size) {
        // move to the last entry
        cerr << checked_mem  << "||" << dataBlock->cast2type<dir_entry>()->rec_len << endl;
        auto rec = entry_list -> rec_len;
        checked_mem += rec;
        // move pointer
        entry_list = (dir_entry*) ((char*) entry_list + rec); 
    }
    auto last_entry = dataBlock->cast2type<dir_entry>();
    size_t l_r_l = calcRecLen(last_entry);

    // round rec_len & l_r_l to %4
    _4(l_r_l);
    _4(rec_len);

    if (checked_mem + l_r_l + rec_len > block_size) {
        return false;
    }

    // update last one
    last_entry->rec_len = l_r_l;

    // move to available last directory entry.
    last_entry = (dir_entry*) ((char*) last_entry + l_r_l);
    // arrange newly put dir entry.
    last_entry->rec_len = block_size - checked_mem - l_r_l;
    dataBlock->movep(l_r_l);

    // put last dirEntry.
    avail_dir_entry = dataBlock;

    return true;
}


INode *lookupINode (const uint& inodeNo) {

    auto total_inode = GET(superBlock, s_inodes_count);
    

    if (inodeNo > total_inode) {
        _exit(EXIT_FAILURE);
    }
    
    // calculate group and idx of the inode
    auto inode_per_group = GET(superBlock, s_inodes_per_group);
    uint group_of_inode = (inodeNo - 1)/ inode_per_group;
    uint idx_of_inode = (inodeNo -1) % inode_per_group;

    // fetch the corresponding group
    GroupDesc *gd = new GroupDesc(desc);
    gd->move(group_of_inode);

    // get inode table of the group
    INode* inode = new INode(GET(gd, bg_inode_table), INodeSize);

    // delete gd from memory
    delete gd;

    // move to the required inode
    inode->move(idx_of_inode);
    inode->get();
    return inode;
}

void writeFile (string fn, uint iNodeNo) {

    auto buf = getStat(fn);
    uint mode = buf->st_mode; // uint
    cerr << "writeFile triggered" << endl;
    TODO (
        if (S_ISREG(mode)) {
            cout << "Regular file given" << endl;
        }
        else {
            cout << "Directory given" << endl;
        }
    )
    INode* inode = lookupINode(iNodeNo);
    uint fInodeNo;
    INode *fInode = (INode*) findFreeINodeBlock(&fInodeNo);
    SET(fInode, i_blocks, 0);
    SET(fInode, i_mode, buf->st_mode);
    SET(fInode, i_size, buf->st_size);
    SET(fInode, i_uid, LOW16(buf->st_uid)); 
    SET(fInode, i_atime, buf->st_atime);
    SET(fInode, i_ctime, buf->st_ctime);
    SET(fInode, i_mtime, buf->st_mtime);
    SET(fInode, i_dtime, 0);
    SET(fInode, i_gid, LOW16(buf->st_gid));
    SET(fInode, i_links_count, 1);
    fInode->save();
    putFile2DataBlock(fn, fInode);


    //rec_len of the new dirEntry.
    size_t dir_length = sizeof(int) + sizeof(short) + 2 * sizeof(char) + fn.length() * sizeof(char);

    uint *blocks = GET(inode, i_block);
    Block *avail_dir_entry = nullptr;
    bool indirect = true;
    for (char b = 0; b < 12; b++) {
        if (checkDirEntry(blocks[b], dir_length, avail_dir_entry)) {
            indirect = false;
            break;
        }
    }
    IGNORE (
        if (indirect) {
            auto depth = 1;
            // search indirect pointers
            while (indirectDirEntry (blocks[depth + 12], dir_length, avail_dir_entry, depth) && depth++ < 3);
        }
    )

    fillDirectoryEntry(avail_dir_entry, fn, fInodeNo);
}

void init_vars (const string& imageFile) {
    OpenExt2(imageFile);
    // get SuperBlock from first group.
    superBlock = new SuperBlock(1, SuperBlockSize);

    // Calculate block size.
    block_size = EXT2_BLOCK_SIZE << GET(superBlock, s_log_block_size);

    // get group descriptors
    desc = new GroupDesc(2, GroupDescSize);
    desc->get();

    num_of_block_groups = (uint) ceil(GET(superBlock, s_blocks_count) / (double) GET(superBlock, s_blocks_per_group));
}

int main(int argc, char* args[]) {
    if (argc != 4) {
        cerr << "INSUFFICIENT ARGUMENT!" << endl;
        cerr << "PROGRAM WILL ABORT IMMEDIATELY!" << endl;
        _exit(EXIT_FAILURE); 
    }
    string imageFile = args[1];
    string sourceFile = args[2];
    string target = args[3];
    init_vars(imageFile);

    char msg[512];
    sprintf(msg, "Block Size: %d, Block Count: %d, Inode Size: %d, Inode Count: %d, number of block groups: %d \n",
                                            block_size, GET(superBlock, s_blocks_count), 
                                            GET(superBlock, s_inode_size), GET(superBlock, s_inodes_count), num_of_block_groups);
    cerr << msg << endl;
    
    cerr << GET(desc, bg_block_bitmap) << "|"
        << GET(desc, bg_inode_bitmap) << "|"
        << GET(desc, bg_free_blocks_count) << "|"
        << GET(desc, bg_free_inodes_count) << "|"
        << GET(desc, bg_used_dirs_count) << "|" << endl;
    if (target[0] <= '9' && target[0] >= '0') {
        // Target is INode
        cerr << "Target Inode fetched" << endl;
        writeFile(sourceFile, stoi(target));
    }
    else if (target[0] == '/') {

    }

    delete superBlock;
    delete desc;
}