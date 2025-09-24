#include <stdio.h>
#include "main.h"
#include "menu.h"

int main(int argc, char *argv[])
{
    int choice;

    do {
        show_menu();
        scanf("%d", &choice);
        // Clear the input buffer
        while (getchar() != '\n'); 
        handle_option(choice);
    } while (choice != 0);

    return 0;
}