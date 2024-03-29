#include <string.h>

#include "./headers/consts.h"
#include "./headers/tm_tools.h"

void clear_screen()
{
#ifdef __linux__
    system("clear");
#else
    system("cls");
#endif
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
    usleep(WAIT_TIME * 100);
}

void print_config_error()
{
    printf("_________________________________________________________________\n");
    printf("WARNING: CONFIG NOT FOUND. THE MACHINE IS SET TO DEFAULT SETTINGS\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
}

void print_config_loading_error()
{
    printf("______________________________________________________________________________\n");
    printf("ERROR: THE CONFIG FILE CONTAINS ERRORS. THE MACHINE IS SET TO DEFAULT SETTINGS\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
}

void print_help()
{
    printf("\nPlease specify the input tape and the instruction by using the input files in the input folder. \nYou can specify and empty character on the tape using '-', while the END command using 'E'. Enjoy!\n\n");
    printf("------------\nLIST OF COMMANDS:\n");
    printf("LOAD: Load tape and instructions from file\nPRINT [CONTENT]: Print loaded data. Arguments can be TAPE or INST (instructions)\n");
    printf("RUN: Execute the program\nHELP: Get this message\nABOUT: Something about this program\nCLEAR: Clear the screen from previous outputs\nSETTINGS: Open settings dialog\nEXIT: Exit the program\n-----------\nCommands and case-insensitive\n");
}

void print_about()
{
    printf("\nA C implementation of a Universal Turing Machine!\n\n-------------------------------------------------------\nFork on GitHub: https://github.com/tiz314/TuringMachine\n-------------------------------------------------------\n");
    printf("Made by Tiziano Radicchi (Tiz314) @SSRI UniMI 2022\n\n");
}

void print_settings()
{
    printf("\n1- Change TM mode\n2- Step by step Execution\n3- Exit settings\n");
}

void print_changing_tm_mode()
{
    printf("One tape (1)\nTwo tapes (2)\n");
}

void print_changing_stbst_exec()
{
    printf("Enabled (1)\nDisabled (2)\n");
}

void print_mode_change_success(char *input)
{
    printf("________________________________________\n");
    printf("MODE SUCCESFULLY SET TO %s TAPE MODE\n", input);
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
}

void print_mode_unchanged()
{
    printf("________________\n");
    printf("MODE ALREADY SET\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
}

void print_loading_warning()
{
    printf("______________________________________________________________________________________________________________\n");
    printf("WARNING: TAPE AND/OR INSTRUCTIONS FILE CONTAINS ERRORS OR HAS NOT BEEN FOUND! HAVE YOU FORGOTTEN TO LOAD THEM?\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
}

void print_error_during_loading()
{
    printf("________________________________________________________________________\n");
    printf("ERROR: INSTRUCTIONS NOT LOADED CORRECTLY. CHECK THE SYNTAX FOR ANY ERROR\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
}

void print_command_not_found()
{
    printf("____________________________________________________________\n");
    printf("ERROR: COMMAND NOT FOUND. TYPE \"HELP\" FOR A LIST OF COMMANDS\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
}

void print_tape(cell *c, int tape_number)
{
    cell *cp = get_first_element(c);
    printf("\n#%d: ", tape_number);
    if (cp->element || cp->next != NULL)
    {
        printf("|  ");

        while (cp != NULL)
        {
            if (cp->element == 0)
            {
                printf("   |  ");
            }
            else
            {
                printf("%c  |  ", cp->element);
            }
            cp = (cell *)cp->next;
        }
    }
    else
    {
        printf("Tape is empty");
    }
    printf("\n");
}

void print_instructions(char **instructions, int instructions_n, int mode)
{
    int i, j, size = mode ? TWO_TAPES_RULES_NUMBER : SINGLE_TAPE_RULES_NUMBER;
    printf("\n=====================");
    if (mode)
    {
        printf("============");
    }
    printf("\n");
    for (i = 0; i < instructions_n; i++)
    {
        for (j = 0; j < size; j++)
        {
            printf("| %c ", instructions[i][j]);
        }
        printf("|\n");
    }
    printf("=====================");
    if (mode)
    {
        printf("============");
    }
    printf("\n");
}

void print_machine_iteration(char status, cell *c, int tape_number)
{
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("STATUS: %c\n", status);
    print_tape(c, tape_number);
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

void print_no_instruction_found_alert()
{
    printf("_______________________________________\n");
    printf("WARNING: NO NEW INSTRUCTIONS WERE FOUND\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
}

void print_warning_no_instructions()
{
    printf("________________________________________________\n");
    printf("WARNING: THE FILE DOES NOT CONTAIN INSTRUCTIONS!\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
}

void print_success_loading()
{
    printf("________________________\n");
    printf("FILES SUCCESFULLY LOADED\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
}

void print_exiting()
{
    printf("______________________________\n");
    printf("YOU ARE EXITING THE PROGRAM...\n");
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
}

/* ASCII ART printed on boot()

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
