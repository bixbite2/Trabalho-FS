#include "ls.h"
#include <stdio.h>
#include "create_fs.h"

extern const char* FS_FILENAME;

void ls() {
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

    printf("Arquivos no diretório raiz:\n");
    for (int i = 0; i < sizeof(root_dir) / sizeof(DirEntry); ++i) {
        if (root_dir[i].in_use) {
            printf("- %s (tamanho: %u bytes)\n", root_dir[i].name, root_dir[i].size);
        }
    }

    fclose(fs_file);
}

