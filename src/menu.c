#include "menu.h"
#include <stdio.h>
#include "create_fs.h"
#include "cp_in.h"
#include "cp_out.h"
#include "rename_file.h"
#include "rm.h"
#include "ls.h"
#include "df.h"

void show_menu() {
    printf("\n--- File System Menu ---\n");
    printf("1. Criar File System (create_fs)\n");
    printf("2. Copiar para o FS (cp_in)\n");
    printf("3. Copiar para fora do FS (cp_out)\n");
    printf("4. Renomear arquivo (rename)\n");
    printf("5. Remover arquivo (rm)\n");
    printf("6. Listar arquivos (ls)\n");
    printf("7. Mostrar espaço livre (df)\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

void handle_option(int option) {
    switch (option) {
        case 1:
            create_fs_ui();
            break;
        case 2:
            cp_in_ui();
            break;
        case 3:
            cp_out_ui();
            break;
        case 4:
            rename_ui();
            break;
        case 5:
            rm_ui();
            break;
        case 6:
            ls_ui();
            break;
        case 7:
            df_ui();
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opção inválida!\n");
            break;
    }
}

void cp_in_ui() {
    cp_in();
}

void cp_out_ui() {
    cp_out();
}

void rename_ui() {
    rename_file();
}

void rm_ui() {
    rm();
}

void ls_ui() {
    ls();
}

void df_ui() {
    df();
}

void create_fs_ui() {
    create_fs();
}
