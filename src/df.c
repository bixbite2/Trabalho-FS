#include "df.h"
#include <stdio.h>
#include "create_fs.h"

extern const char* FS_FILENAME;

void df() {
    FILE *fs_file = fopen(FS_FILENAME, "rb");
    if (fs_file == NULL) {
        perror("Não foi possível abrir o arquivo do sistema de arquivos");
        return;
    }

    Superblock sb;
    fread(&sb, sizeof(Superblock), 1, fs_file);

    uint32_t fat[sb.fat_block_count * sb.block_size / sizeof(uint32_t)];
    fseek(fs_file, sb.fat_start_block * sb.block_size, SEEK_SET);
    fread(fat, sizeof(fat), 1, fs_file);

    uint32_t free_blocks = 0;
    for (uint32_t i = sb.data_start_block; i < sb.block_count; ++i) {
        if (fat[i] == 0) {
            free_blocks++;
        }
    }

    long long int free_space = (long long int)free_blocks * sb.block_size;
    long long int used_space = sb.fs_size - free_space;

    printf("Espaço total: %lu bytes\n", (unsigned long)sb.fs_size);
    printf("Espaço usado: %lu bytes\n", (unsigned long)used_space);
    printf("Espaço livre: %lu bytes\n", (unsigned long)free_space);

    fclose(fs_file);
}

