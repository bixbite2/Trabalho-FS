#include "rm.h"
#include <stdio.h>
#include <string.h>
#include "create_fs.h"

extern const char* FS_FILENAME;

void rm(const char *filename) {
    FILE *fs_file = fopen(FS_FILENAME, "r+b");
    if (fs_file == NULL) {
        perror("Não foi possível abrir o arquivo do sistema de arquivos");
        return;
    }

    Superblock sb;
    fread(&sb, sizeof(Superblock), 1, fs_file);

    fseek(fs_file, sb.root_dir_start_block * sb.block_size, SEEK_SET);
    DirEntry root_dir[sb.root_dir_block_count * sb.block_size / sizeof(DirEntry)];
    fread(root_dir, sizeof(root_dir), 1, fs_file);

    int dir_entry_index = -1;
    for (int i = 0; i < sizeof(root_dir) / sizeof(DirEntry); ++i) {
        if (root_dir[i].in_use && strcmp(root_dir[i].name, filename) == 0) {
            dir_entry_index = i;
            break;
        }
    }

    if (dir_entry_index == -1) {
        printf("Arquivo '%s' não encontrado.\n", filename);
        fclose(fs_file);
        return;
    }

    uint32_t fat[sb.fat_block_count * sb.block_size / sizeof(uint32_t)];
    fseek(fs_file, sb.fat_start_block * sb.block_size, SEEK_SET);
    fread(fat, sizeof(fat), 1, fs_file);

    uint32_t current_block = root_dir[dir_entry_index].start_block;
    while (current_block != 0xFFFFFFFF) {
        uint32_t next_block = fat[current_block];
        fat[current_block] = 0;
        current_block = next_block;
    }

    root_dir[dir_entry_index].in_use = 0;

    fseek(fs_file, sb.root_dir_start_block * sb.block_size, SEEK_SET);
    fwrite(root_dir, sizeof(root_dir), 1, fs_file);

    fseek(fs_file, sb.fat_start_block * sb.block_size, SEEK_SET);
    fwrite(fat, sizeof(fat), 1, fs_file);

    fclose(fs_file);

    printf("Arquivo '%s' removido com sucesso.\n", filename);
}

