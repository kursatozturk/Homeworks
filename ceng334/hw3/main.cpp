#include "main.hpp"

using namespace std;

uint block_size = 0;

#define BLOCK_OFFSET(block) (BASE_OFFSET + (block-1)*block_size)
#define BLOCK_COUNT superBlock->s_blocks_count
#define INODE_SIZE superBlock->s_inode_size
#define INODE_COUNT superBlock->s_inodes_count
#define NUM_OF_BLOCK_GROUPS (uint)ceil(superBlock->s_blocks_count / (double) superBlock->s_blocks_per_group)
#define INODE_GROUP(inode) (inode - 1) / superBlock->s_inodes_per_group
#define INODE_IDX(inode) (inode - 1) % superBlock->s_inodes_per_group
 
#define GET_OFFSET(block) (ext32File + BLOCK_OFFSET (block))

// File Holder
char *ext32File = nullptr;
size_t file_size = 0;
// superBlock ptr
SuperBlock *superBlock = nullptr;

// group descriptor ptr
GroupDesc *desc = nullptr;

// root INode ptr
INode *root = nullptr;


void readEXT32File (const string &imagePath) {

    // read Content of File System
    long fs;
    ext32File = readContent(imagePath.c_str(), &fs);

    file_size = fs;

    // read superBlock of first Block Group
    superBlock = (SuperBlock *) (ext32File + BASE_OFFSET);
    // set block_size immediately
    block_size = pow(2, superBlock->s_log_block_size + 10);

    desc = (GroupDesc *) GET_OFFSET(2);
    root = (INode *) GET_OFFSET(desc->bg_inode_table) + 1;
}

INode* lookupINode (const uint& inode_no) {
    if (inode_no > INODE_COUNT) {
        cerr << "IT IS BEYOND INODES." << endl;
        TODO (
            SAVE IMAGE
        )

        _exit(EXIT_FAILURE);
    }
    uint group = INODE_GROUP(inode_no);
    uint INode_idx = INODE_IDX(inode_no);
    
    // set the corresponding group's descriptor
    GroupDesc* gd = desc + group;

    // set table_ptr to beginning of INode Tables
    INode *table_ptr = (INode*) GET_OFFSET(gd->bg_inode_table);
    
    TODO (
        If target inode is not allocated,
        throw not allocated error. 
    )

    // return corresponding Inode
    return table_ptr + INode_idx;
}

INode* findFreeINodeBlock (uint *id = nullptr) {

    INode* availBlock = nullptr;
    for (size_t group = 0; group < NUM_OF_BLOCK_GROUPS; ++group){
        // traverse through groups
        auto gd = desc + group;
        // check block bitmap
        bmap *inodeMap = (bmap*) GET_OFFSET(gd->bg_inode_bitmap);
        
        // find table
        INode *inode_table = (INode*) GET_OFFSET(gd->bg_inode_table);

        for (uint i = 0; i < superBlock->s_inodes_per_group; i++) {
            if (!(BM_ISSET(i, inodeMap))) {
                //found available data_block
                availBlock = inode_table + i;
                if (id)
                    *id = i + 1;
                // mark bitmap to state block is reserved.
                BM_SET(i, inodeMap);
                superBlock->s_free_inodes_count--;
                gd->bg_free_inodes_count--;
                return availBlock;
            }
        }
    }
    return nullptr;
}

void* findFreeDataBlock (uint *id = nullptr) {

    void* availBlock = nullptr;
    for (size_t group = 0; group < NUM_OF_BLOCK_GROUPS; ++group){
        // traverse through groups
        auto gd = desc + group;
        // check block bitmap
        bmap *blockMap = (bmap*) GET_OFFSET(gd->bg_block_bitmap);
        
        // find the idx of first data_block of group
        uint data_block = gd->bg_inode_table
                    + ceil(superBlock->s_inodes_per_group * sizeof(INode) / block_size);
        // calculate data block offset
        uint group_offset = group * superBlock->s_blocks_per_group;
        uint blockmap_offset = data_block - group_offset;

        for (uint i = blockmap_offset; i < superBlock->s_blocks_per_group; i++) {
            if (!(BM_ISSET(i, blockMap))) {
                //found available data_block
                availBlock = (void*) GET_OFFSET(i + 1 + group_offset);
                if (id)
                    *id = i + group_offset + 1;
                // mark bitmap to state block is reserved.
                BM_SET(i, blockMap);
                superBlock->s_free_blocks_count--;
                superBlock->s_r_blocks_count++;
                gd->bg_free_blocks_count--;
                // initialized 0 to data block.
                memset(availBlock, 0, block_size);
                return availBlock;
            }
        }
    }
    return nullptr;
}

int indirect_link2Inode(uint* block_ids, const uint& id, unsigned char depth) {
    /*
        block_ids: Holds link ids. If depth == 1, links are direct(namely, directly points data),
                else(depth > 1) links are indirect.        
        id: index of the free data block found. It will be put to first appropriate place.
        depth: holds the depth of indirect links. i.e. : 
                        if is a single indirect link, depth==1
                        if is a doubly indirect link, depth==2
    */

    if (depth == 1) {
        for(uint i = 0; i < block_size / sizeof(uint); ++i) {
            if (!block_ids[i]){
                // put newly created data block id
                block_ids[i] = id;
                return i;
            }
        }
    }

    else {
        for (uint i = 0; i < block_size / sizeof(uint); ++i) {
            uint* indirect_block_ids = nullptr;
            if (!block_ids[i]) {
                // if no block is reserved,
                // reserve a block for indirect pointer.
                indirect_block_ids = (uint* ) findFreeDataBlock(&block_ids[i]);
            }
            else {
                indirect_block_ids = (uint* ) GET_OFFSET(block_ids[i]);
            }
            auto idx = indirect_link2Inode(indirect_block_ids, id, depth-1);
            if (idx != -1)
                return idx;
        }
    }
    return -1;
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
    cout << "link_block2Inode triggered" << endl;
    int dblock_id = -1;
    for (char i = 0; i < 12; ++i)
            if(!fInode->i_block[i]){
                dblock_id = i;
                fInode->i_block[i] = id;
                break;
            }

    if (dblock_id == -1) {
        int depth = 1;
        do {
            uint* indirect_block_ids = nullptr;
            if (!fInode->i_block[11 + depth]) {
                // if no block is reserved for indirect links,
                // reserve a block.
                indirect_block_ids = (uint*) findFreeDataBlock(&fInode->i_block[11 + depth]);
            }
            else {
                indirect_block_ids = (uint*) GET_OFFSET(fInode->i_block[11 + depth]);
            }

            dblock_id = indirect_link2Inode(indirect_block_ids, id, depth);
            depth++;
        }while (dblock_id == -1 && depth <= 3);
    }
    if (dblock_id == -1) {
        TODO (
            throw error. NO AVAILABLE MEMORY.
        )
        return;
    }
}

void putFile2DataBlock (
    const string& fn,  // name of the file

    INode* fInode    // inode block of file
                    // where inode of file resides.
) {
    long fs;
    char *file_content = readContent(fn, &fs);
    char *d = file_content;
    do {
        uint d_id;
        // get free Data Block
        char* dataBlock = (char*) findFreeDataBlock(&d_id);
        cout << fn << " file written to block: " << d_id << "..." << endl;
        // put pointer(id) to i_block of fINode
        link_block2Inode(fInode, d_id);

        // a block is reserved for INode
        // increase counter.
        fInode->i_blocks += block_size/512; // not sure about 512
        // determine how many byte will be written to block
        auto write_byte = min((uint)fs, block_size);
        fs -= block_size;
        // copy file to dataBlock
        memcpy(dataBlock, d, write_byte);
        d += write_byte;
    } while(fs > 0);
    delete[] file_content;
}

bool fillDirectoryEntry (
    DirEntry* &availBlock, // DirEntry to be filled
    const string& fn, // name of the file

    uint& inodeNo    // no of inode block
                    // where inode of file resides.
    ) {
    size_t dir_length = sizeof(int) + sizeof(short) + 2 * sizeof(char) + fn.length();
    availBlock->name_len = fn.length();
    FillStr(availBlock->name, fn);
    availBlock->inode = inodeNo;

    auto buf = getStat(fn);
    
    availBlock->file_type = S_ISREG(buf->st_mode)?EXT2_FT_REG_FILE:EXT2_FT_DIR;

    return true;
}

bool checkDirEntry(uint dataBlockId, size_t rec_len, DirEntry* &avail_dir_entry) {
    TODO (
        if dataBlockId == 0:
            find a free dataBlock.
            write it to that.
    )
    char* dir_list = (char *) GET_OFFSET(dataBlockId);
    uint checked_mem = 0;
    while (checked_mem + ((DirEntry*) dir_list) -> rec_len < block_size) {
        // move to the last entry
        checked_mem += ((DirEntry*) dir_list) -> rec_len;
        dir_list += ((DirEntry*) dir_list) -> rec_len;
    }

    DirEntry *last_dir = (DirEntry *)dir_list;
    size_t l_r_l = calcRecLen(last_dir);

    // round l_r_l to %4
    _4(l_r_l);

    checked_mem += l_r_l;
    if (checked_mem + l_r_l + rec_len > block_size) {
        return false;
    }

    // update last one
    last_dir->rec_len = l_r_l;

    // put last dirEntry.
    avail_dir_entry = (DirEntry*) (dir_list + l_r_l);

    // arrange newly put dir entry.
    avail_dir_entry->rec_len = block_size - checked_mem;

    return true;
}

void writeFile (
    string fn, // file name

    uint iNodeNo // inode No to write
    ) {
        
    auto buf = getStat(fn);
    uint mode = buf->st_mode; // uint

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
    fInode->i_blocks = 0;
    fInode->i_mode = buf->st_mode; //???
    fInode->i_size = buf->st_size;
    fInode->i_uid = LOW16(buf->st_uid);
    fInode->i_atime = buf->st_atime;
    fInode->i_ctime = buf->st_ctime;
    fInode->i_mtime = buf->st_mtime;
    fInode->i_dtime = 0;
    fInode->i_gid = LOW16(buf->st_gid);
    fInode->i_links_count = 1;
    
    putFile2DataBlock(fn, fInode);


    //rec_len of the new dirEntry.
    size_t dir_length = sizeof(int) + sizeof(short) + 2 * sizeof(char) + fn.length() * sizeof(char);

    uint *blocks = inode->i_block;
    DirEntry *avail_dir_entry = nullptr;
    for (char b = 0; b < 12; b++)
        if (checkDirEntry(blocks[b], dir_length, avail_dir_entry)) break;
    TODO (
        if cannot find available mem
        do indirect skills of you.
    )
    fillDirectoryEntry(avail_dir_entry, fn, fInodeNo);
};

void write_block_content (INode* f) {
    for (unsigned short i = 0; i < 12; i++) {
        cout << "Block " << i << " : " << f->i_block[i] << endl;
    }
    cout << "Single: " << f->i_block[12] << endl;
    cout << "Double: " << f->i_block[13] << endl;
    cout << "Triple: " << f->i_block[14] << endl;
    cout << "________________________________" << endl;
}

void traverse_through_inodes () {
    for (auto group = 0; group < NUM_OF_BLOCK_GROUPS; ++group) {
        auto gd = desc + group;
        bmap* inodeBitmap = (bmap* ) GET_OFFSET(gd->bg_inode_bitmap);
        bitset <16> reg_bits(EXT2_S_IFDIR), dir_bits(EXT2_S_IFREG);
        cout << reg_bits << " is the reg bits "<< endl;
        cout << dir_bits << " is the dir bits " << endl; 
        for (auto id = 0; id < superBlock->s_inodes_per_group; ++id) {
            if (BM_ISSET(id, inodeBitmap)) {
                INode* root = lookupINode(id + 1 + group * superBlock->s_inodes_per_group);
                cout << "Block content of inode " << id + 1 << endl;
                write_block_content(root);

                bitset<sizeof(unsigned short) * 8> i_mode(root->i_mode);
                cout << "Content of i_mode: " << endl;
                cout << i_mode << endl;
                if (root->i_mode & EXT2_S_IFDIR) {
                    cout << "It is a directory." << endl;
                    cout << "Dive into it.." << endl;
                }
                else if (root->i_mode & EXT2_S_IFREG) {
                    cout << "This block holds a regular file." << endl;
                    cout << "Skipping ..."  << endl;
                    continue;
                }
                else {
                    cout << "UNKNOWN FILE TYPE" << endl;
                    continue;
                }
                for(int i = 0; i < 12; ++i) {
                    if(root->i_block[i]){
                        auto directory = (DirEntry*) GET_OFFSET(root->i_block[i]);
                        uint mem_checked = 0;
                        while (mem_checked < block_size) {
                            cout << "rec_len: " << directory->rec_len << "|dir: " << directory->name << endl;
                            auto _s = directory->rec_len;
                            directory = (DirEntry*) ((char*)directory + _s);
                            mem_checked += _s;
                        }
                    }
                }
            }
        }
    }
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

    readEXT32File(imageFile);
    bmap* block = (bmap*) (ext32File + BLOCK_OFFSET(desc->bg_block_bitmap));
    for (int bi = 0; bi < superBlock->s_blocks_per_group / 8; ++bi){
        bitset<8> bit(block[bi]);
        cout << bit;
    }
    cout << endl;
    char MsgRead[512];
    sprintf(MsgRead, "Block Size: %d, Block Count: %d, Inode Size: %d, Inode Count: %d, number of block groups: %d \n",
                                            block_size, BLOCK_COUNT, INODE_SIZE, INODE_COUNT, NUM_OF_BLOCK_GROUPS);
    cout << MsgRead;
    
    if (target[0] <= '9' && target[0] >= '0') {
        // Target is INode
        writeFile(sourceFile, stoi(target));
    }
    else if (target[0] == '/') {

    }

    traverse_through_inodes();

    FILE *f = fopen(imageFile.c_str(), "wb");
    fwrite(ext32File, 1, file_size, f);
    fclose(f);
    return 0;
}