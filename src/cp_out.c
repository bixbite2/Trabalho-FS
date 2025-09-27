#include "cp_out.h"
#include <stdio.h>
#include <string.h>
#include "create_fs.h"

extern const char* FS_FILENAME;

void cp_out(const char *source_filename, const char *dest_path) {
    FILE *fs_file = fopen(FS_FILENAME, "rb");
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
        if (root_dir[i].in_use && strcmp(root_dir[i].name, source_filename) == 0) {
            dir_entry_index = i;
            break;
        }
    }

    if (dir_entry_index == -1) {
        printf("Arquivo '%s' não encontrado no sistema de arquivos.\n", source_filename);
        fclose(fs_file);
        return;
    }

    FILE *dest_file = fopen(dest_path, "wb");
    if (dest_file == NULL) {
        perror("Não foi possível criar o arquivo de destino");
        fclose(fs_file);
        return;
    }

    uint32_t fat[sb.fat_block_count * sb.block_size / sizeof(uint32_t)];
    fseek(fs_file, sb.fat_start_block * sb.block_size, SEEK_SET);
    fread(fat, sizeof(fat), 1, fs_file);

    char buffer[sb.block_size];
    uint32_t current_block = root_dir[dir_entry_index].start_block;
    long remaining_size = root_dir[dir_entry_index].size;

    while (current_block != 0xFFFFFFFF && remaining_size > 0) {
        fseek(fs_file, current_block * sb.block_size, SEEK_SET);
        long bytes_to_read = (remaining_size > sb.block_size) ? sb.block_size : remaining_size;
        fread(buffer, 1, bytes_to_read, fs_file);
        fwrite(buffer, 1, bytes_to_read, dest_file);
        remaining_size -= bytes_to_read;
        current_block = fat[current_block];
    }

    fclose(fs_file);
    fclose(dest_file);

    printf("Arquivo '%s' copiado para '%s'.\n", source_filename, dest_path);
}

