#include "cp_in.h"
#include <stdio.h>
#include "create_fs.h"

extern const char* FS_FILENAME;

void cp_in(const char *source_path, const char *dest_filename) {
    FILE *fs_file = fopen(FS_FILENAME, "r+b");
    if (fs_file == NULL) {
        perror("Não foi possível abrir o arquivo do sistema de arquivos");
        return;
    }

    Superblock sb;
    fread(&sb, sizeof(Superblock), 1, fs_file);

    FILE *source_file = fopen(source_path, "rb");
    if (source_file == NULL) {
        perror("Não foi possível abrir o arquivo de origem");
        fclose(fs_file);
        return;
    }

    fseek(source_file, 0, SEEK_END);
    long source_size = ftell(source_file);
    fseek(source_file, 0, SEEK_SET);

    uint32_t num_blocks_needed = (source_size + sb.block_size - 1) / sb.block_size;

    fseek(fs_file, sb.root_dir_start_block * sb.block_size, SEEK_SET);
    DirEntry root_dir[sb.root_dir_block_count * sb.block_size / sizeof(DirEntry)];
    fread(root_dir, sizeof(root_dir), 1, fs_file);

    int dir_entry_index = -1;
    for (int i = 0; i < sizeof(root_dir) / sizeof(DirEntry); ++i) {
        if (root_dir[i].in_use == 0) {
            dir_entry_index = i;
            break;
        }
    }

    if (dir_entry_index == -1) {
        printf("Diretório raiz está cheio.\n");
        fclose(source_file);
        fclose(fs_file);
        return;
    }

    uint32_t fat[sb.fat_block_count * sb.block_size / sizeof(uint32_t)];
    fseek(fs_file, sb.fat_start_block * sb.block_size, SEEK_SET);
    fread(fat, sizeof(fat), 1, fs_file);

    uint32_t free_blocks[num_blocks_needed];
    int blocks_found = 0;
    for (uint32_t i = sb.data_start_block; i < sb.block_count && blocks_found < num_blocks_needed; ++i) {
        if (fat[i] == 0) {
            free_blocks[blocks_found++] = i;
        }
    }

    if (blocks_found < num_blocks_needed) {
        printf("Não há espaço suficiente no disco.\n");
        fclose(source_file);
        fclose(fs_file);
        return;
    }

    char buffer[sb.block_size];
    for (int i = 0; i < num_blocks_needed; ++i) {
        fread(buffer, 1, sb.block_size, source_file);
        fseek(fs_file, free_blocks[i] * sb.block_size, SEEK_SET);
        fwrite(buffer, 1, sb.block_size, fs_file);
    }

    strcpy(root_dir[dir_entry_index].name, dest_filename);
    root_dir[dir_entry_index].in_use = 1;
    root_dir[dir_entry_index].start_block = free_blocks[0];
    root_dir[dir_entry_index].size = source_size;

    for (int i = 0; i < num_blocks_needed; ++i) {
        if (i < num_blocks_needed - 1) {
            fat[free_blocks[i]] = free_blocks[i+1];
        } else {
            fat[free_blocks[i]] = 0xFFFFFFFF;
        }
    }

    fseek(fs_file, sb.root_dir_start_block * sb.block_size, SEEK_SET);
    fwrite(root_dir, sizeof(root_dir), 1, fs_file);

    fseek(fs_file, sb.fat_start_block * sb.block_size, SEEK_SET);
    fwrite(fat, sizeof(fat), 1, fs_file);

    fclose(source_file);
    fclose(fs_file);

    printf("Arquivo '%s' copiado para o sistema de arquivos com o nome '%s'.\n", source_path, dest_filename);
}

