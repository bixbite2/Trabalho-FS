#ifndef MENU_H
#define MENU_H

#include "main.h"

void show_menu();
void handle_option(int option);

// Funções para as opções do menu
void create_fs_ui();
void cp_in_ui();
void cp_out_ui();
void rename_ui();
void rm_ui();
void ls_ui();
void df_ui();

#endif