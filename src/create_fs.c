#include "create_fs.h"

const char* FS_FILENAME = "filesystem.fs";

void create_fs(long long int fs_size) {
    FILE *file = fopen(FS_FILENAME, "wb");
    if (file == NULL) {
        perror("Falha ao criar o arquivo do sistema de arquivos");
        return;
    }

    if (ftruncate(fileno(file), fs_size) != 0) {
        perror("Falha ao definir o tamanho do sistema de arquivos");
        fclose(file);
        return;
    }

    Superblock sb;
    strcpy(sb.magic, "FURGFS3");
    sb.version = 1;
    sb.fs_size = fs_size;
    sb.block_size = BLOCK_SIZE;
    sb.block_count = fs_size / BLOCK_SIZE;

    sb.fat_start_block = 1;
    sb.fat_block_count = (sb.block_count * sizeof(uint32_t)) / BLOCK_SIZE;
    if ((sb.block_count * sizeof(uint32_t)) % BLOCK_SIZE != 0) {
        sb.fat_block_count++;
    }

    sb.root_dir_start_block = sb.fat_start_block + sb.fat_block_count;
    sb.root_dir_block_count = 1; 

    sb.data_start_block = sb.root_dir_start_block + sb.root_dir_block_count;
    sb.data_block_count = sb.block_count - sb.data_start_block;

    fseek(file, 0, SEEK_SET);
    fwrite(&sb, sizeof(Superblock), 1, file);

    uint32_t *fat = calloc(sb.fat_block_count * BLOCK_SIZE, 1);
    if (fat == NULL) {
        perror("Falha ao alocar memória para a FAT");
        fclose(file);
        return;
    }
    fat[0] = 0xFFFFFFFF; // Superblock
    for (uint32_t i = 0; i < sb.fat_block_count; ++i) {
        fat[sb.fat_start_block + i] = 0xFFFFFFFF;
    }
    fat[sb.root_dir_start_block] = 0xFFFFFFFF; // Root directory

    fseek(file, sb.fat_start_block * BLOCK_SIZE, SEEK_SET);
    fwrite(fat, sb.fat_block_count * BLOCK_SIZE, 1, file);
    free(fat);

    DirEntry *root_dir = calloc(sb.root_dir_block_count * BLOCK_SIZE, 1);
    if (root_dir == NULL) {
        perror("Falha ao alocar memória para o diretório raiz");
        fclose(file);
        return;
    }
    fseek(file, sb.root_dir_start_block * BLOCK_SIZE, SEEK_SET);
    fwrite(root_dir, sb.root_dir_block_count * BLOCK_SIZE, 1, file);
    free(root_dir);

    fclose(file);

    printf("Sistema de arquivos criado com sucesso em %s com tamanho de %lld bytes.\n", FS_FILENAME, fs_size);
}

