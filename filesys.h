/* filesys.h
 *
 * describes FAT structures
 * http://www.c-jump.com/CIS24/Slides/FAT/lecture.html#F01_0020_fat
 * http://www.tavi.co.uk/phobos/fat.html
 */

#ifndef FILESYS_H
#define FILESYS_H

#include <time.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define MAXBLOCKS 1024
#define BLOCKSIZE 1024
#define FATENTRYCOUNT (BLOCKSIZE / sizeof(fatentry_t))
#define DIRENTRYCOUNT ((BLOCKSIZE - (2*sizeof(int))) / sizeof(direntry_t))
#define MAXNAME 256
#define MAXPATHLENGTH 1024

#define UNUSED -1
#define ENDOFCHAIN 0
#define EOF -1

#define DATA 0
// #define FAT 1
#define DIR 2

typedef unsigned char Byte;

// create a type fatentry_t, we set this currently to short (16-bit)
typedef short fatentry_t;

// a FAT block is a list of 16-bit entries that form a chain of disk addresses

//const int   fatentrycount = (blocksize / sizeof(fatentry_t));

typedef struct direntry {
  int         entrylength;   // records length of this entry (can be used with names of variables length)
  Byte        is_dir; // apparently this is redundant, but I don't think it is
  Byte        unused;
  time_t      modtime;
  int         file_length;
  fatentry_t  first_block;
  char   name [MAXNAME];
} direntry_t;

// a directory block is an array of directory entries

//const int   direntrycount = (blocksize - (2*sizeof(int))) / sizeof(direntry_t);

typedef fatentry_t fatblock_t[FATENTRYCOUNT];

// create a type direntry_t

typedef struct dirblock {
  int is_dir;
  int next_entry;
  direntry_t entrylist[DIRENTRYCOUNT]; // the first two integer are marker and endpos
} dirblock_t;

// a data block holds the actual data of a file_length, it is an array of 8-bit (byte) elements

typedef Byte datablock_t[BLOCKSIZE];

// a diskblock can be either a directory block, a FAT block or actual data

typedef union block {
  datablock_t data;
  dirblock_t  dir;
  fatblock_t  fat;
} diskblock_t;

// finally, this is the disk: a list of diskblocks
// the disk is declared as extern, as it is shared in the program
// it has to be defined in the main program file_length

extern diskblock_t virtual_disk[MAXBLOCKS];

// when a file is opened on this disk, a file handle has to be
// created in the opening program

typedef struct filedescriptor {
  int         pos;           // byte within a block
  char        mode[3]; //file mode, r w etc
  Byte        writing;
  fatentry_t  blockno; //no, this is the index of the current block
  diskblock_t buffer;
} my_file_t;

void format();
void write_disk(const char * file_name);
void save_file();

// file handling functions
my_file_t *myfopen(char *path, char *mode);
char myfgetc(my_file_t *file);
int myfputc(char character, my_file_t *file);
int myfclose(my_file_t *file);
void print_block(int block_index, char type);
void print_fat(int length);
void create_file();
void print_directory_structure(int current_dir_block, int indent);
void mymkdir(char *path);
char **mylistdir(char *path);
int dir_index_for_path(char *path);
void print_dir_list(char **list);
void mychdir(char *path);
void current();
char **path_to_array(char *path);
char *last_entry_in_path(char **path);
int number_of_entries_in_path(char **path);
void myremove( char * path);
int file_entry_index(char *filename);
void myrmdir(char *path);

#endif

/*
#define NUM_TYPES (sizeof types / sizeof types[0])
static* int types[] = {
    1,
    2,
    3,
    4 };
*/
