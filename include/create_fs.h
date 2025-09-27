#ifndef CREATE_FS_H
#define CREATE_FS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLOCK_SIZE 4096

typedef struct {
    char magic[8]; // "FURGFS3"
    uint32_t version;
    uint64_t fs_size;
    uint32_t block_size;
    uint32_t block_count;
    uint32_t fat_start_block;
    uint32_t fat_block_count;
    uint32_t root_dir_start_block;
    uint32_t root_dir_block_count;
    uint32_t data_start_block;
    uint32_t data_block_count;
} Superblock;

typedef struct {
    uint8_t in_use;
    char name[55];
    uint32_t start_block;
    uint32_t size;
} DirEntry;

void create_fs(long long int fs_size);

#endif