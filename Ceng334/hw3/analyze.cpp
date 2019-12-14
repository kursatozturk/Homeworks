#include "tools.hpp"
#include <sstream>

using namespace std;

uint block_size = 0;
#define BLOCK_OFFSET(block) (BASE_OFFSET + (block-1)*block_size)

char *ext32File = nullptr;

void analyze (const string& imagePath) {

    
    // read Content of File System
    ext32File = readContent(imagePath.c_str());

    // read superBlock of first Block Group
    auto superBlock = (SuperBlock *) (ext32File + BASE_OFFSET);
    // set block_size immediately
    block_size = pow(2, superBlock->s_log_block_size + 10);

    auto desc = (GroupDesc *) (ext32File + BLOCK_OFFSET(2));
    auto root = (INode *) (ext32File + BLOCK_OFFSET(desc->bg_inode_table)) + 1;

        unsigned int   s_inodes_count;      /* Inodes count */
        unsigned int   s_blocks_count;      /* Blocks count */
        unsigned int   s_r_blocks_count;    /* Reserved blocks count */
        unsigned int   s_free_blocks_count; /* Free blocks count */
        unsigned int   s_free_inodes_count; /* Free inodes count */
        unsigned int   s_first_data_block;  /* First Data Block */
    stringstream msg;
    msg << "Total Inode Count: " << superBlock->s_inodes_count << "\n"
        << "Total Block Count: "




}


int main () {

    return 0;
}
