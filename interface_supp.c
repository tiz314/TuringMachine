#include <stdio.h>

#ifdef __linux__ // including libraries for sleep, based on platform on which you compile
#include <unistd.h>
#else
#include <windows.h>
#endif

void clear_screen()
{
#ifdef __linux__

    printf("\033[2J");
    printf("\033[0;0f");
#else
    clrscr();
#endif
}

void print_menu()
{
    printf("\nSELECT AN OPTION: \n\n1- LOAD FILES\n2- PRINT TAPE\n3- PRINT INSTRUCTIONS\n4- EXECUTE\n5- HELP\n6- ABOUT\n7- SHUTDOWN\n\n> ");
}

void print_boot()
{
    clear_screen();
    printf("==============================================================================================================================\n");
    printf(" /$$$$$$$$                  /$$                           /$$      /$$                     /$$       /$$                    \n");
    printf("|__  $$__/                 |__/                          | $$$    /$$$                    | $$      |__/                    \n");
    printf("   | $$ /$$   /$$  /$$$$$$  /$$ /$$$$$$$   /$$$$$$       | $$$$  /$$$$  /$$$$$$   /$$$$$$$| $$$$$$$  /$$ /$$$$$$$   /$$$$$$ \n");
    printf("   | $$| $$  | $$ /$$__  $$| $$| $$__  $$ /$$__  $$      | $$ $$/$$ $$ |____  $$ /$$_____/| $$__  $$| $$| $$__  $$ /$$__  $$\n");
    printf("   | $$| $$  | $$| $$  \\__/| $$| $$  \\ $$| $$  \\ $$      | $$  $$$| $$  /$$$$$$$| $$      | $$  \\ $$| $$| $$  \\ $$| $$$$$$$$\n");
    printf("   | $$| $$  | $$| $$      | $$| $$  | $$| $$  | $$      | $$\\  $ | $$ /$$__  $$| $$      | $$  | $$| $$| $$  | $$| $$_____/\n");
    printf("   | $$|  $$$$$$/| $$      | $$| $$  | $$|  $$$$$$$      | $$ \\/  | $$|  $$$$$$$|  $$$$$$$| $$  | $$| $$| $$  | $$|  $$$$$$$\n");
    printf("   |__/ \\______/ |__/      |__/|__/  |__/ \\____  $$      |__/     |__/ \\_______/ \\_______/|__/  |__/|__/|__/  |__/ \\_______/\n");
    printf("                                          /$$  \\ $$                                                                         \n");
    printf("                                         |  $$$$$$/                                                                         \n");
    printf("                                          \\______/                                                                          \n");
    printf("==============================================================================================================================\n");
    sleep(1);
}

void print_help()
{
    printf("\nPlease specify the input tape and the instruction by using the input files in the input folder. \nYou can specify and empty character on the tape using '-', while the END command using 'E'. Enjoy!\n\n");
}

void print_about()
{
    printf("\nA C implementation of a Universal Turing Machine!\n\n-------------------------------------------------------\nFork on GitHub: https://github.com/tiz314/TuringMachine\n-------------------------------------------------------\n");
    printf("Made by Tiz314, Mighty Gibbon and Staring Linus :|\n\n");
}

void print_loading_warning()
{
    printf("______________________________________________________________________________________________________________\n");
    printf("WARNING: TAPE AND/OR INSTRUCTIONS FILE CONTAINS ERRORS OR HAS NOT BEEN FOUND! HAVE YOU FORGOTTEN TO LOAD THEM?\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
}

void print_tape(char tape[])
{
    int i, tape_dim = strlen(tape);
    printf("\n|  ");
    for (i = 0; i < tape_dim; i++)
    {
        if (tape[i] == '*')
            printf("   |  ");
        else
            printf("%c  |  ", tape[i]);
    }
    printf("\n");
}

void print_instructions(char instructions[MAX_INSTRUCTIONS][RULES_NUMBER], int instructions_n)
{
    int i, j;
    printf("\n=====================\n");
    for (i = 0; i < instructions_n; i++)
    {
        for (j = 0; j < RULES_NUMBER; j++)
        {
            printf("| %c ", instructions[i][j]);
        }
        printf("|\n");
    }
    printf("=====================\n");
}

void print_machine_iteration(char status, int tape_position, char tape[])
{
    printf("----------------------------------------------------\n");
    printf("STATUS: %c\tTAPE POSITION: %d\n", status, tape_position);
    print_tape(tape);
    printf("----------------------------------------------------\n");
}

/* ASCII ART

 /$$$$$$$$                  /$$                           /$$      /$$                     /$$       /$$
|__  $$__/                 |__/                          | $$$    /$$$                    | $$      |__/
   | $$ /$$   /$$  /$$$$$$  /$$ /$$$$$$$   /$$$$$$       | $$$$  /$$$$  /$$$$$$   /$$$$$$$| $$$$$$$  /$$ /$$$$$$$   /$$$$$$
   | $$| $$  | $$ /$$__  $$| $$| $$__  $$ /$$__  $$      | $$ $$/$$ $$ |____  $$ /$$_____/| $$__  $$| $$| $$__  $$ /$$__  $$
   | $$| $$  | $$| $$  \__/| $$| $$  \ $$| $$  \ $$      | $$  $$$| $$  /$$$$$$$| $$      | $$  \ $$| $$| $$  \ $$| $$$$$$$$
   | $$| $$  | $$| $$      | $$| $$  | $$| $$  | $$      | $$\  $ | $$ /$$__  $$| $$      | $$  | $$| $$| $$  | $$| $$_____/
   | $$|  $$$$$$/| $$      | $$| $$  | $$|  $$$$$$$      | $$ \/  | $$|  $$$$$$$|  $$$$$$$| $$  | $$| $$| $$  | $$|  $$$$$$$
   |__/ \______/ |__/      |__/|__/  |__/ \____  $$      |__/     |__/ \_______/ \_______/|__/  |__/|__/|__/  |__/ \_______/
                                          /$$  \ $$
                                         |  $$$$$$/
                                          \______/

*/
