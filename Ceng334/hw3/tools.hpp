#ifndef TOOLS_HPP
#define TOOLS_HPP
extern "C" {
    #include "ext21.h"
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/stat.h>
}

#include <cmath>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <bitset>

using namespace std;

#define BASE_OFFSET 1024
#define EXT2_BLOCK_SIZE 1024

typedef unsigned char bmap;
#define __NBITS (8 * (int) sizeof (bmap))
#define __BMELT(d) ((d) / __NBITS)
#define __BMMASK(d) ((bmap) 1 << ((d) % __NBITS))
#define BM_SET(d, set) ((set[__BMELT (d)] |= __BMMASK (d)))
#define BM_CLR(d, set) ((set[__BMELT (d)] &= ~__BMMASK (d)))
#define BM_ISSET(d, set) ((set[__BMELT (d)] & __BMMASK (d)) != 0)
typedef struct stat Stat;

#define IGNORE(...)

IGNORE(
    typedef struct ext2_super_block SuperBlock;
    typedef struct ext2_group_desc GroupDesc;
    typedef struct ext2_inode INode;
    typedef struct ext2_dir_entry DirEntry;
    typedef struct stat Stat;

)

#define _4(r) while(r%4)r++
#define LOW16(r) r & 0xFFFF

#define FillStr(t, s) {\
                        int i = 0;\
                        for(auto const &c: s) {\
                            t[i++] = c;\
                        }\
                    }

#define TODO(...)

inline char* readContent(const string& fn, long *fs=nullptr) {
    // open file
    FILE *f = fopen(fn.c_str(), "rb");

    // get size of file
    fseek (f, 0, SEEK_END); 
    auto fSize = ftell (f);
    if (fs) {
        *fs = fSize;
    }
    rewind (f);

    //fill buffer with content of file 
    char *content = new char[fSize];
    fread(content, 1, fSize, f);

    // close file
    fclose(f);
    
    return content; 
}


inline Stat* getStat (const string& fn) {
    struct stat *buf = new struct stat();
    if (stat(fn.c_str(), buf) == -1) {
        cerr << "stat buf cannot be read!" << endl;
        TODO (
            save image
        )
        _exit(EXIT_FAILURE);
    }
    return buf;
}


inline size_t calcRecLen(struct ext2_dir_entry* dir) {
    size_t rec_len = sizeof(int) + sizeof(short) + 2 * sizeof(char);
    char *name = dir->name;
    while (*name++)rec_len += sizeof(char);
    return rec_len + sizeof(char);
}

#endif // TOOLS.HPP