#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "./headers/consts.h"

#include "./headers/tm_tools.h"
#include "./headers/cli_interface.h"

int main(int argc, char const *argv[])
{
    __uint8_t check_end, check_menu = 1, loaded = 0, i, j; // check if END command reached; check for the menu cycle; check for file loading; general purpose counters
    char user_choice[BUFSIZ];                              // user input in the menu. pre-defined size because of the possibility to enter string commands

    __uint8_t m_mode;                  // Machine mode of operation: it can be 0 (one tape machine) or 1 (two tapes machine)
    char status;                       // current machine status, starts from 0
    unsigned short int instructions_n; // number of received instructions
    unsigned short int pos;            // positive counter for the position in the instruction matrix

    char **instructions; // char pointer for dynamic matrix of instructions

    tape main_tape;             // TM main tape. Not dinamically allocated because of the possibility to write new slots by the program itself
    char backup_tape[TAPE_DIM]; // backup char array. The original tape is copied also here, in order to recover it without realoding from file. Out of the struct because not always necessary, but only for the main tape

    // ** ONLY FOR TWO TAPES MODE, IF ENABLED **
    tape *second_tape; // Eventually used pointer to tape struct, containing a char array and an int used as position counter
    // **

    char c;           // char use to read single chars from files
    FILE *input_file; // opens read only input.txt and instructions.txt
    FILE *instructions_file;
    // FILE *config_file = fopen("./settings.conf", "rw"); // Config file, contains information about the configuration of the machine

    m_mode = 0; // Setting the single tape mode as default

    print_boot();

    /*if (config_file == NULL) // config file loading process, handling possible errors
    {
        print_config_error();
        //set_default_settings(&m);
    }
    else
    {
        if (load_settings(config_file, &m) == -1)
        {
            print_config_loading_error();
          //  set_default_settings(&m);
        }
    }*/

    while (check_menu)
    {
        printf(PROMPT);

        scanf("\n"); // Removing from buffer any residuals from previous input. Necessary for settings dialog inputs
        fgets(user_choice, BUFSIZ, stdin);
        user_choice[strlen(user_choice) - 1] = '\0'; // receveing user input and removing \n for strcmp. Fgets is necessary because some commands include spaces, not included by scanf function

        for (int i = 0; user_choice[i]; i++)
        {
            user_choice[i] = toupper(user_choice[i]); // uppering the input
        }

        if (!strcmp(user_choice, "LOAD"))
        {
            input_file = fopen(TAPE_PATH, "r");
            instructions_file = fopen(INSTR_PATH, "r");
            if (instructions_file == NULL || input_file == NULL)
            {
                print_loading_warning();
                loaded = 0;
            }
            else
            {
                if (loaded)
                {
                    loaded = 0;                          // unloading instructions
                    for (i = 0; i < instructions_n; i++) // removing from the heap the dynamically allocated instructions matrix
                    {
                        free(instructions[i]);
                    }
                    free(instructions);
                    init_tape(backup_tape); // Clearing backup tape from residuals, if any
                    if (m_mode == 1)        // if two tapes activated, also init the second tape
                        init_tape(second_tape->content);
                }

                check_end = 0;                                // check used to control the execution of the program. Initialized here and repeated every time before possible execution
                instructions_n = 0;                           // resetting the counter
                fscanf(input_file, "%s", main_tape.content);  // reads until breakline is reached
                while ((c = fgetc(instructions_file)) != EOF) // Instructions counting
                {                                             // read character by character and check if the end of the file is reached
                    if (c == '\n')
                    {
                        instructions_n++;
                    }
                }

                instructions_n++; // counter is increased by 1 since the last '\n' is lost. Placed here outside the if in case of just one instruction.
                if (instructions_n > 0)
                {
                    rewind(instructions_file); // rewinding file for instructions reading

                    instructions = (char **)calloc(instructions_n, sizeof(char *)); // allocating the char arrays array
                    if (m_mode == 0)
                    {
                        for (i = 0; i < instructions_n; i++)
                        {
                            instructions[i] = (char *)calloc(SINGLE_TAPE_RULES_NUMBER, sizeof(char)); // allocating the single char array, which is equal to an instruction
                        }
                    }
                    else
                    {
                        for (i = 0; i < instructions_n; i++)
                        {
                            instructions[i] = (char *)calloc(TWO_TAPES_RULES_NUMBER, sizeof(char)); // allocating the single char array, which is equal to an instruction
                        }
                    }

                    j = 1; // Using GP counter j for checking the pattern response from file content

                    if (m_mode == 0)
                    {
                        for (i = 0; i < instructions_n && j; i++) // filling instructions matrix for single tape machine
                        {
                            if (fscanf(instructions_file, "(%c, %c, %c, %c, %c)\n", &instructions[i][0], &instructions[i][1], &instructions[i][2], &instructions[i][3], &instructions[i][4]) != 5)
                            {
                                j = 0;
                            }
                        }
                    }
                    else
                    {
                        for (i = 0; i < instructions_n && j; i++) // filling instructions matrix for two tapes machine
                        {
                            if (fscanf(instructions_file, "(%c, %c, %c, %c, %c, %c, %c, %c)\n", &instructions[i][0], &instructions[i][1], &instructions[i][2], &instructions[i][3], &instructions[i][4], &instructions[i][5], &instructions[i][6], &instructions[i][7]) != 8)
                            {
                                j = 0;
                            }
                        }
                    }

                    if (j)
                    {
                        print_success_loading();
                        // initializing struct variables
                        status = '0';
                        main_tape.pos = 0;
                        loaded = 1;                                // setting file check for other operations
                        copy_tape(main_tape.content, backup_tape); // backup of the tape
                    }
                    else
                    {
                        print_error_during_loading();
                    }
                }
                else
                {
                    print_warning_no_instructions();
                    loaded = 0;
                }
            }
        }
        else if (!strcmp(user_choice, "PRINT TAPE"))
        {
            if (loaded)
            {
                print_tape(main_tape.content);
                if (m_mode == 1) // If two tapes enable, also print the second tape
                    print_tape(second_tape->content);
            }
            else
            {
                print_loading_warning();
            }
        }
        else if (!strcmp(user_choice, "PRINT INST"))
        {

            if (loaded)
            {
                print_instructions(instructions, instructions_n, m_mode);
            }
            else
            {
                print_loading_warning();
            }
        }
        else if (!strcmp(user_choice, "RUN"))
        {
            if (loaded)
            {
                print_machine_iteration(instructions[pos][0], main_tape.pos, main_tape.content); // First tape print. Useful if any instruction won't be chosen
                if (m_mode == 0)                                                                 // If single mode chosen, use this algorithm
                {
                    do
                    {
                        pos = 0;
                        while (
                            (pos < instructions_n) &&
                            (main_tape.pos < TAPE_DIM) &&
                            (instructions[pos][0] != status || instructions[pos][1] != main_tape.content[main_tape.pos]) &&
                            !(instructions[pos][0] == status && instructions[pos][1] == '-' && (main_tape.content[main_tape.pos] == '\0' || main_tape.content[main_tape.pos] == '*' || main_tape.content[main_tape.pos] == ' '))) // if looking for - on tape but reaching end of char array or space (human mane (' ') or machine made('*'))
                        {
                            pos++;
                        }

                        if (pos < instructions_n && main_tape.pos < TAPE_DIM)
                        {
                            if (instructions[pos][2] == 'E')
                                check_end = 1; // if END reached, unflag check and exit loop
                            else
                                status = instructions[pos][2];

                            if (instructions[pos][3] == '-')
                                main_tape.content[main_tape.pos] = '*'; // I can understand if it is a machine made space
                            else
                                main_tape.content[main_tape.pos] = instructions[pos][3];

                            if (instructions[pos][4] == '>')
                                main_tape.pos++;
                            else if (instructions[pos][4] == '<')
                                main_tape.pos--;

                            print_machine_iteration(instructions[pos][0], main_tape.pos, main_tape.content);
                        }
                        else
                        {
                            check_end = 1; // Tape end reached or no instructions to execute
                        }
                    } while (pos < instructions_n && !check_end);
                }
                else if (m_mode == 1)
                { // Else if two tapes (not else, maybe more tapes in the future), use this algorithm, which includes the additional tape. Another algorithm is required, since even the structure of instructions differs.
                    // TODO
                    do
                    {
                        pos = 0;
                        while (
                            (pos < instructions_n) &&
                            (main_tape.pos < TAPE_DIM) &&
                            (instructions[pos][0] != status || instructions[pos][1] != main_tape.content[main_tape.pos] || instructions[pos][2] != second_tape->content[second_tape->pos]) &&
                            !(instructions[pos][0] == status && instructions[pos][1] == '-' && (main_tape.content[main_tape.pos] == '\0' || main_tape.content[main_tape.pos] == '*' || main_tape.content[main_tape.pos] == ' '))) // if looking for - on tape but reaching end of char array or space (human mane (' ') or machine made('*'))
                        {
                            pos++;
                        }

                        if (pos < instructions_n && main_tape.pos < TAPE_DIM)
                        {
                            if (instructions[pos][2] == 'E')
                                check_end = 1; // if END reached, unflag check and exit loop
                            else
                                status = instructions[pos][2];

                            if (instructions[pos][3] == '-')
                                main_tape.content[main_tape.pos] = '*'; // I can understand if it is a machine made space
                            else
                                main_tape.content[main_tape.pos] = instructions[pos][3];

                            if (instructions[pos][4] == '>')
                                main_tape.pos++;
                            else if (instructions[pos][4] == '<')
                                main_tape.pos--;

                            print_machine_iteration(instructions[pos][0], main_tape.pos, main_tape.content);
                        }
                        else
                        {
                            check_end = 1; // Tape end reached or no instructions to execute
                        }
                    } while (pos < instructions_n && !check_end);
                }

                if (pos == instructions_n)
                {
                    print_no_instruction_found_alert();
                }

                pos = 0;                                   // Restoring pos. Necessary for any outdated value from previous executions. If tape changes in size, then segfault would occur
                check_end = 0;                             // Restoring check end
                status = '0';                              // Restoring machine status
                main_tape.pos = 0;                         // Restoring tape position
                copy_tape(backup_tape, main_tape.content); // restoring the tape
            }
            else
            {
                print_loading_warning();
            }
        }
        else if (!strcmp(user_choice, "HELP"))
        {
            print_help();
        }
        else if (!strcmp(user_choice, "ABOUT"))
        {
            print_about();
        }
        else if (!strcmp(user_choice, "CLEAR"))
        {
            clear_screen();
        }
        else if (!strcmp(user_choice, "SETTINGS"))
        {
            print_settings();
            printf(PROMPT);
            scanf("\n%s", user_choice);
            if (user_choice[0] == '1')
            {
                print_changing_tm_mode();
                printf(PROMPT);
                scanf("\n%s", user_choice);

                if (user_choice[0] == '1') // switching between modes of operation (single tape of two tapes)
                {
                    if (m_mode == 1) // If the previous mode was two tapes, a reload of instructions is required
                        loaded = 0;
                    m_mode = 0;
                    print_mode_change_success("SINGLE");
                    if (second_tape != NULL)
                    {
                        free(second_tape); // freeing space allocated for the second tape
                    }
                }
                else if (user_choice[0] == '2')
                {
                    m_mode = 1;
                    print_mode_change_success("TWO");
                    second_tape = (tape *)malloc(sizeof(tape));
                    init_tape(second_tape->content); // Second tape init
                    loaded = 0;                      // Refresh of loaded instructions required. To avoid that older single tape instructions could be processed as two tapes and generate false positives about syntax errors
                }
                else
                    printf("Invalid option\n");
            }
            else
            {
                printf("\nExiting settings...\n");
            }
        }
        else if (!strcmp(user_choice, "EXIT"))
        {
            if (instructions_file)
                fclose(instructions_file);
            if (input_file)
                fclose(input_file); // Cosing files

            print_exiting();
            check_menu = 0;
        }
        else
        {
            print_command_not_found();
        }
    }

    return 0;
}
