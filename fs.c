#include "fdc.h"
#include "string.h"
#include "mem.h"
#include "fs.h"

struct FileEntry* get_file(int drive, char* filename2) {
    // allocate memory for file table
    char* filetable =  malloc(1024);
    // read file table into memory
    read_blocks(2, drive, 1, filetable);
    // loop over file entries
    for (int i = 0; i < 32; i++) {
        struct FileEntry* fileentry = (struct FileEntry*) filetable + (i * 32);
        // if the entry's filename is the one we want
        if (strcmp(fileentry->filename, filename2)) {
            // return it
            return fileentry;
        }
    }
    return 0;
}

void create_file(int drive, char* filename2) {
    // allocate memory for file table
    char* filetable =  malloc(1024);
    // read file table into memory
    read_blocks(2, drive, 1, filetable);
    // select available file entry
    struct FileEntry* fileentry = (struct FileEntry*) filetable + (3 * 32);
    // set data
    strcpy(filename2, fileentry->filename);
    fileentry->lba = 0x7eadbeef;
    fileentry->size = 0x7afebabe;
    // write table back to disk
    write_blocks(2, drive, 1, filetable);
}