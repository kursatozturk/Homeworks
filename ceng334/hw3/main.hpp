#include "tools.hpp"

uint block_size = 0;
#define BLOCK_OFFSET(block) (BASE_OFFSET + (block-1)*block_size)
uint num_of_block_groups = 0;

class __ext2File {
    public:
        static const string& __set_fn__ (const string fn = "") {
            static const string _fn(fn);
            return _fn;
        }
        static int __get__ () {
            static int __ext2 = open(__ext2File::__set_fn__().c_str(), O_RDWR);
            return __ext2;
        }
    private:
        __ext2File () {};
        __ext2File (__ext2File&) = delete;
        void operator=(__ext2File&) = delete;
    public:
        ~__ext2File () {}
};

#define OpenExt2(fn) __ext2File::__set_fn__(fn)
#define ext2File __ext2File::__get__()

template<class T>
class __ext2_structures {
protected:
    int __ext2;
    size_t _offset;
    size_t _t_size;
    T *__s; // holds beginning of data block on memory
    T *pointer; // traverser
    size_t __moved;
    bool changed;
    bool in_memory;
public:
    __ext2_structures (size_t offset){
        /*
            offset of the structure must be given
            to initialize structure.
        */
        // open file

        this->__ext2 = ext2File;

        this->_offset = BLOCK_OFFSET(offset);
        this->_t_size = sizeof(T);

        // allocate memory for struct
        this->__s = (T*) malloc(this->_t_size);
        this->pointer = this->__s;
        this->__moved = 0;
        this->changed = false;
        this->in_memory = false;
    }
    __ext2_structures ( size_t offset,
                        size_t T_size){
        /*
            offset of the structure must be given
            to initialize structure.
        */
        // open file

        this->__ext2 = ext2File;

        this->_offset = BLOCK_OFFSET(offset);
        this->_t_size = T_size;

        // allocate memory for struct
        this->__s = (T*) malloc(T_size);
        this->pointer = this->__s;
        this->__moved = 0;
        this->changed = false;
        this->in_memory = false;
    
    }
    __ext2_structures (__ext2_structures& rhs) {
        // copy features
        this->_offset = rhs._offset;
        this->_t_size = rhs._t_size;
        // initialize file pointer
        this->__ext2 = ext2File;
        // allocate memory for struct
        this->__s = (T*) malloc(this->_t_size);
        this->pointer = this->__s;
        this->__moved = 0;
        this->changed = rhs.changed;
        if (rhs.in_memory && !rhs.changed) {
            this->in_memory = true;
            memcpy(this->__s, rhs.__s, rhs._t_size);

        }
        else {
            this->in_memory = false;
        }
    }
    
    __ext2_structures (const __ext2_structures* const rhs) {
        // copy features
        this->_offset = rhs->_offset;
        this->_t_size = rhs->_t_size;
        // initialize file pointer
        this->__ext2 = ext2File;
        // allocate memory for struct
        this->__s = (T*) malloc(this->_t_size);
        this->pointer = this->__s;
        this->__moved = 0;
        this->changed = rhs->changed;
        if (rhs->in_memory && !rhs->changed) {
            this->in_memory = true;
            memcpy(this->__s, rhs->__s, rhs->_t_size);

        }
        else {
            this->in_memory = false;
        }
    }

    void save () {
        if (this->in_memory) {
            lseek(this->__ext2, this->_offset, SEEK_SET);
            write(this->__ext2, this->pointer, this->_t_size);
        }
    }

    T *get(size_t read_byte = 0) {
        // NOTE: if content changed,
        // it must be saved manually. 
        // Otherwhise it may be discarded.
        // ######################################################
        // if not read_byte specified and has not changed
        // return current pointer
        if (!read_byte && !this->changed && this->in_memory)
            return this->pointer;
        // if read_byte is specified or
        // content has changed, read from disk
        auto __read = read_byte == 0? this->_t_size: read_byte;
        //std::cerr << "reading  with offset: " << this->_offset << " and size: " << __read << std::endl;
        if (__read != this->_t_size){
            // overwrite _t_size to save correctly
            this->__s = (T*) realloc(this->__s, __read);
            this->_t_size = __read;
        }
        this->pointer = this->__s;
        this->__moved = 0;

        // read struct from file
        lseek(this->__ext2, this->_offset, SEEK_SET);
        read(this->__ext2, this->__s, __read);
        this->pointer = this->__s;
        this->changed = false;
        this->in_memory = true;
        return this->pointer;
    }


    void move (size_t move_size) {
        // moves cursor on file
        this->_offset += move_size * this->_t_size;
        this->changed = true;
    }
    
    void movep (size_t move_size) {
        // moves the pointer holding data
        if (this->__moved  + move_size >= this->_t_size) {
            cerr << "reached end of block" << endl;
            return;
        }
        cerr << "moving pointer" << endl;
        this->pointer = (T*) move_size; // let the C++ do the magic.
        this->__moved += move_size;
        this->changed = true; // mark content has changed.

    }

    void setOffset (size_t offset) {
        // set offset on disk.
        this->_offset = BLOCK_OFFSET(offset);
        this->changed = true; // mark content has changed.
    }

    void blockWrite (size_t write_byte, char* _target) {
        // write as whole block.
        // it is designed for DataBlock usages
        // where template parameter is char.

        //Realloc memory for block write.
        this->__s = (T*) realloc(this->__s, write_byte);
        this->pointer = this->__s;
        this->__moved = 0;
        this->_t_size = write_byte;
        // get content of target
        memcpy(this->__s, _target, write_byte);
        // save data to disk
        this->save();
        this->changed = true;
    }
    template<class T2>
    T2* cast2type () {
        return (T2*) this->get();
    }
    ~__ext2_structures () {
        delete this->__s;
    }
};

#define FREE(i, bm) !BM_ISSET(i, bm->get())
#define RESERVE(i, bm) BM_SET(i, bm->get())

#define GET(base, attr) base->get()->attr
#define GETINCR(base, inc) base->get() + inc

#define SET(base, attr, value) base->get()->attr=value

#define CAST(_t) __ext2_structures<_t>

typedef CAST(struct ext2_super_block) SuperBlock;
typedef CAST(struct ext2_group_desc) GroupDesc;
typedef CAST(struct ext2_inode) INode;
typedef CAST(struct ext2_dir_entry) DirEntry;
typedef CAST(bmap) BitMap;
typedef CAST(char) Block;

#define SuperBlockSize sizeof(struct ext2_super_block)
#define GroupDescSize sizeof(struct ext2_group_desc)
#define INodeSize sizeof(struct ext2_inode)
#define DirEntrySize block_size // it must be read by a pointer.
#define BitMapSize sizeof(bmap)


SuperBlock *superBlock = nullptr;

GroupDesc *desc = nullptr;