#include "menu.h"
#include <stdio.h>

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
    printf("Função 'Copiar para o FS' chamada.\n");
    // Lógica para cp_in aqui
}

void cp_out_ui() {
    printf("Função 'Copiar para fora do FS' chamada.\n");
    // Lógica para cp_out aqui
}

void rename_ui() {
    printf("Função 'Renomear arquivo' chamada.\n");
    // Lógica para rename aqui
}

void rm_ui() {
    printf("Função 'Remover arquivo' chamada.\n");
    // Lógica para rm aqui
}

void ls_ui() {
    printf("Função 'Listar arquivos' chamada.\n");
    // Lógica para ls aqui
}

void df_ui() {
    printf("Função 'Mostrar espaço livre' chamada.\n");
    // Lógica para df aqui
}

void create_fs_ui() {
    printf("Função 'Criar File System' chamada.\n");
    // Lógica para create_fs aqui
}
