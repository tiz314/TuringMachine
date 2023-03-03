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

    __uint8_t m_mode, exec_mode, config_load_success = 1; // Machine mode of operation: it can be 0 (one tape machine) or 1 (two tapes machine); Exec mode. It can be 0 (continuous) or 1 (step by step)
    char status;                                          // current machine status, starts from 0
    unsigned short int instructions_n;                    // number of received instructions
    unsigned short int pos;                               // positive counter for the position in the instruction matrix

    char **instructions; // char pointer for dynamic matrix of instructions

    cell *tape_cell_temp; // used as temporary variable for prev pointer while adding an element
    cell *main_tape_cell = (cell *)calloc(1, sizeof(cell));
    cell *main_tape_backup = (cell *)calloc(1, sizeof(cell)); // useful to avoid requiring reload after execs

    // ** ONLY FOR TWO TAPES MODE, IF ENABLED **
    cell *second_tape_cell; // Eventually used pointer to tape struct, containing a char array and an int used as position counter
    // **

    char c;           // char use to read single chars from files
    FILE *input_file; // opens read only input.txt and instructions.txt
    FILE *instructions_file; // file pointer for instructions file
    FILE *config_file = fopen("../settings.conf", "rw"); // Config file, contains information about the configuration of the machine

    print_boot();

    if (config_file == NULL) // config file loading process, handling possible errors
    {
        print_config_error();
        config_load_success = 0;
    }
    else
    {
        if (fscanf(config_file, "tmmode: %hhd\n", &m_mode) != 1 || (m_mode != 0 && m_mode != 1))
        {
            config_load_success = 0;
        }
        if (fscanf(config_file, "execmode: %hhd", &exec_mode) != 1 || (exec_mode != 0 && exec_mode != 1))
        {
            config_load_success = 0;
        }
        if (!config_load_success)
        {
            print_config_loading_error();
        }
    }
    if (!config_load_success)
    {
        m_mode = 0;    // Setting the single tape mode as default
        exec_mode = 0; // Setting the continuous mode as default
    }

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

                    init_tape(main_tape_cell); // Clearing backup tape from residuals, if any
                    if (m_mode == 1)           // if two tapes activated, also init the second tape
                        init_tape(second_tape_cell);
                }

                check_end = 0;      // check used to control the execution of the program. Initialized here and repeated every time before possible execution
                instructions_n = 0; // resetting the counter

                fill_tape(input_file, main_tape_cell); // filling the tape

                while ((c = fgetc(instructions_file)) != EOF) // Instructions counting
                {                                             // read character by character and check if the end of the file is reached
                    if (c == '\n')
                    {
                        instructions_n++; // if the end of line is reached, another instruction must be counted
                    }
                }

                instructions_n++; // counter is increased by 1 since the last '\n' is lost. Placed here outside the if in case of just one instruction.
                if (instructions_n > 0)
                {
                    rewind(instructions_file); // rewinding file for instructions reading

                    instructions = (char **)malloc(instructions_n * sizeof(char *)); // allocating the char arrays array
                    if (m_mode == 0)
                    {
                        for (i = 0; i < instructions_n; i++)
                        {
                            instructions[i] = (char *)malloc(SINGLE_TAPE_RULES_NUMBER * sizeof(char)); // allocating the single char array, which is equal to an instruction
                        }
                    }
                    else
                    {
                        for (i = 0; i < instructions_n; i++)
                        {
                            instructions[i] = (char *)malloc(TWO_TAPES_RULES_NUMBER * sizeof(char)); // allocating the single char array, which is equal to an instruction
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
                        loaded = 1; // setting file check for other operations

                        copy_tape(main_tape_cell, main_tape_backup); // Backup of the tape
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
                print_tape(main_tape_cell, 1);
                if (m_mode == 1)
                { // If two tapes enable, also print the second tape
                    print_tape(second_tape_cell, 2);
                }
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
                print_machine_iteration(instructions[pos][0], main_tape_cell, 1); // First tape print. Useful if any instruction won't be chosen
                if (m_mode == 0)                                                  // If single mode chosen, use this algorithm
                {

                    do
                    {
                        if (exec_mode)
                        { // If step by step enabled:
                            printf("Press enter to continue ('q' to terminate)\n");
                            fgets(user_choice, BUFSIZ, stdin);
                            if (user_choice[0] == 'q')
                            {
                                check_end = 1;
                            }
                        }
                        if (!check_end) // To avoid executing useless code if user decided to exit
                        {
                            pos = 0;
                            while (
                                (pos < instructions_n) &&
                                (instructions[pos][0] != status || instructions[pos][1] != main_tape_cell->element) &&
                                !(instructions[pos][0] == status && instructions[pos][1] == '-' && (main_tape_cell->element == '\0'))) // if looking for - on tape but reaching end of char array or space (human mane (' ') or machine made('*'))
                            {
                                pos++;
                            }

                            if (pos < instructions_n)
                            {
                                if (instructions[pos][2] == 'E')
                                    check_end = 1; // if END reached, unflag check and exit loop
                                else
                                    status = instructions[pos][2];

                                if (instructions[pos][3] == '-')
                                    main_tape_cell->element = '\0'; // I can understand if it is a machine made space. In this way, i can simplify if statements looking for empty cell by looking for \0 in middle tape or on the right written edge (actual escape char). Middle spaces filled with \0 by the "fill tape" function in tm_tools.c
                                else
                                    main_tape_cell->element = instructions[pos][3];

                                if (instructions[pos][4] == '>')
                                {
                                    if (main_tape_cell->next == NULL)
                                    {
                                        main_tape_cell->next = (struct cell *)calloc(1, sizeof(cell));
                                    }
                                    tape_cell_temp = main_tape_cell;
                                    main_tape_cell = (cell *)main_tape_cell->next;
                                    main_tape_cell->prev = (struct cell *)tape_cell_temp;
                                }
                                else if (instructions[pos][4] == '<')
                                {
                                    if (main_tape_cell->prev == NULL)
                                    {
                                        main_tape_cell->prev = (struct cell *)calloc(1, sizeof(cell));
                                    }
                                    tape_cell_temp = main_tape_cell;
                                    main_tape_cell = (cell *)main_tape_cell->prev;
                                    main_tape_cell->next = (struct cell *)tape_cell_temp;
                                }

                                print_machine_iteration(instructions[pos][0], main_tape_cell, 1);
                            }
                            else
                            {
                                check_end = 1; // Tape end reached or no instructions to execute
                            }
                        }
                    } while (pos < instructions_n && !check_end);
                }
                else
                { // Else two tapes (by now, m_mode strictly set to 0 or 1), use this algorithm, which includes the additional tape. Another algorithm is required, since even the structure of instructions differs.
                    init_tape(second_tape_cell);
                    print_machine_iteration(instructions[pos][0], second_tape_cell, 2);
                    printf("\n");

                    do
                    {
                        if (exec_mode)
                        { // If step by step enabled:
                            printf("Press enter to continue ('q' to terminate)\n");
                            fgets(user_choice, BUFSIZ, stdin);
                            if (user_choice[0] == 'q')
                            {
                                check_end = 1;
                            }
                        }
                        if (!check_end)
                        {
                            pos = 0;
                            while (
                                (pos < instructions_n) &&
                                (instructions[pos][0] != status || instructions[pos][1] != main_tape_cell->element || instructions[pos][2] != second_tape_cell->element) &&
                                !(instructions[pos][0] == status && instructions[pos][1] == '-' && (main_tape_cell->element == '\0') && (instructions[pos][2] == second_tape_cell->element || instructions[pos][2] == '-' && (second_tape_cell->element == '\0'))) &&
                                !(instructions[pos][0] == status && instructions[pos][2] == '-' && (second_tape_cell->element == '\0') && (instructions[pos][1] == main_tape_cell->element || instructions[pos][1] == '-' && (main_tape_cell->element == '\0')))) // if looking for - on tape but reaching end of char array or space (human mane (' ') or machine made('*'))
                            {                                                                                                                                                                                                                                     
                                pos++;
                            }
                            if (pos < instructions_n)
                            {
                                if (instructions[pos][3] == 'E')
                                    check_end = 1; // if END reached, unflag check and exit loop
                                else
                                    status = instructions[pos][3];

                                if (instructions[pos][4] == '-')    // Second tape input
                                    main_tape_cell->element = '\0'; // I can understand if it is a machine made space
                                else
                                    main_tape_cell->element = instructions[pos][4];

                                if (instructions[pos][5] == '-')      // Second tape input
                                    second_tape_cell->element = '\0'; // I can understand if it is a machine made space
                                else
                                    second_tape_cell->element = instructions[pos][5];

                                if (instructions[pos][6] == '>') // Main tape head movement
                                {
                                    if (main_tape_cell->next == NULL)
                                    {
                                        main_tape_cell->next = (struct cell *)calloc(1, sizeof(cell));
                                    }
                                    tape_cell_temp = main_tape_cell;
                                    main_tape_cell = (cell *)main_tape_cell->next;
                                    main_tape_cell->prev = (struct cell *)tape_cell_temp;
                                }
                                else if (instructions[pos][6] == '<')
                                {
                                    if (main_tape_cell->prev == NULL)
                                    {
                                        main_tape_cell->prev = (struct cell *)calloc(1, sizeof(cell));
                                    }
                                    tape_cell_temp = main_tape_cell;
                                    main_tape_cell = (cell *)main_tape_cell->prev;
                                    main_tape_cell->next = (struct cell *)tape_cell_temp;
                                }

                                if (instructions[pos][7] == '>') // Second tape head movement
                                {
                                    if (second_tape_cell->next == NULL)
                                    {
                                        second_tape_cell->next = (struct cell *)calloc(1, sizeof(cell));
                                    }
                                    tape_cell_temp = second_tape_cell;
                                    second_tape_cell = (cell *)second_tape_cell->next;
                                    second_tape_cell->prev = (struct cell *)tape_cell_temp;
                                }
                                else if (instructions[pos][7] == '<')
                                {
                                    if (second_tape_cell->prev == NULL)
                                    {
                                        second_tape_cell->prev = (struct cell *)calloc(1, sizeof(cell));
                                    }
                                    tape_cell_temp = second_tape_cell;
                                    second_tape_cell = (cell *)second_tape_cell->prev;
                                    second_tape_cell->next = (struct cell *)tape_cell_temp;
                                }

                                print_machine_iteration(instructions[pos][0], main_tape_cell, 1);
                                print_machine_iteration(instructions[pos][0], second_tape_cell, 2);
                                printf("\n");
                            }
                            else
                            {
                                check_end = 1; // Tape end reached or no instructions to execute
                            }
                        }
                    } while (pos < instructions_n && !check_end);
                }

                if (pos == instructions_n)
                {
                    print_no_instruction_found_alert();
                }

                pos = 0;       // Restoring pos. Necessary for any outdated value from previous executions. If tape changes in size, then segfault would occur
                check_end = 0; // Restoring check end
                status = '0';  // Restoring machine status

                main_tape_cell = get_first_element(main_tape_cell); // Resetting the cell pointer to the first cell now in memory. This is the only moment in which this reset is done
                if (m_mode == 1)
                    second_tape_cell = get_first_element(second_tape_cell); // Eventually also for the second tape

                init_tape(main_tape_cell); // clearing the main tape from previous execution. If not, in case of a shorter originale tape, the result would partially remain on the tape and not overwritten!
                copy_tape(main_tape_backup, main_tape_cell);
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
            switch (user_choice[0])
            {
            case '1':
            {
                print_changing_tm_mode();
                printf(PROMPT);
                scanf("\n%s", user_choice);

                if (user_choice[0] == '1') // switching between modes of operation (single tape of two tapes)
                {
                    if (m_mode == 1)
                    { // If the previous mode was two tapes, a reload of instructions is required
                        loaded = 0;
                        free_tape(second_tape_cell); // If switch from two tape to one tape, freeing space used for the second tape
                        m_mode = 0;
                        print_mode_change_success("SINGLE");
                        init_tape(main_tape_cell); // In order to clear the tape from previous executions
                    }
                    else
                    {
                        print_mode_unchanged();
                    }
                }
                else if (user_choice[0] == '2')
                {
                    if (m_mode == 0)
                    {
                        second_tape_cell = (cell *)calloc(1, sizeof(cell)); // Only if actually switching from one tape mode to two tape mode. If not, not doing another malloc
                        m_mode = 1;
                        print_mode_change_success("TWO");
                        loaded = 0; // Refresh of loaded instructions required. To avoid that older single tape instructions could be processed as two tapes and generate false positives about syntax errors
                    }
                    else
                    {
                        print_mode_unchanged();
                    }
                }
                else
                    printf("Invalid option\n");
                break;
            }
            case '2':
            {
                print_changing_stbst_exec();
                printf(PROMPT);
                scanf("\n%s", user_choice);

                if (user_choice[0] == '1') // switching between modes of operation (step by step exec and continuous)
                {
                    if (exec_mode == 0)
                    {
                        exec_mode = 1;
                        printf("Success\n");
                    }
                    else
                    {
                        print_mode_unchanged();
                    }
                }
                else if (user_choice[0] == '2')
                {
                    if (exec_mode == 1)
                    {
                        exec_mode = 0;
                        printf("Success\n");
                    }
                    else
                    {
                        print_mode_unchanged();
                    }
                }
                else
                    printf("Invalid option\n");
                break;
            }
            default:
            {
                printf("\nExiting settings...\n");
            }
            }
        }
        else if (!strcmp(user_choice, "EXIT"))
        {
            if (instructions_file)
            {
                fclose(instructions_file);
            }
            if (input_file)
            {
                fclose(input_file); // Cosing files
            }
            if (config_file)
            {
                fclose(config_file);
            }

            print_exiting();
            check_menu = 0;
            free_tape(main_tape_cell);
            free_tape(main_tape_backup);
            if (m_mode == 1)
                free_tape(second_tape_cell);
        }
        else
        {
            print_command_not_found();
        }
    }

    return 0;
}