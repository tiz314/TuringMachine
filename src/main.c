#include "consts.h"
#include <stdlib.h>
#include <ctype.h>

#include "tm_tools.h"
#include "cli_interface.h"

int main(int argc, char const *argv[])
{
    __uint8_t check_end, check_menu = 1, loaded = 0, i, j; // check if END command reached; check for the menu cycle; check for file loading; general purpose counters
    char user_choice[BUFSIZ];                              // user input in the menu. pre-defined size because of the possibility to enter string commands
    tm_components m;
    char **instructions; // char pointer for dynamic matrix of instructions

    // ** ONLY FOR TWO TAPES MODE, IF ENABLED **
    char *second_tape;        // Eventually used char pointer for the second tape
    int second_tape_position; // Counter for the head position on the second tape. Not included in the machine typedef because not sure if used.
    // **

    char c;           // char use to read single chars from files
    FILE *input_file; // opens read only input.txt and instructions.txt
    FILE *instructions_file;
    // FILE *config_file = fopen("./settings.conf", "rw"); // Config file, contains information about the configuration of the machine

    m.m_mode = 0; // Setting the single tape mode as default

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
                    loaded = 0;                            // unloading instructions
                    for (i = 0; i < m.instructions_n; i++) // removing from the heap the dynamically allocated instructions matrix
                    {
                        free(instructions[i]);
                    }
                    free(instructions);
                    init_tape(m.backup_input); // Clearing backup tape from residuals, if any
                    if (m.m_mode == 1)         // if two tapes activated, also init the second tape
                        init_tape(second_tape);
                }

                check_end = 0;                                // check used to control the execution of the program. Initialized here and repeated every time before possible execution
                m.instructions_n = 0;                         // resetting the counter
                fscanf(input_file, "%s", m.input);            // reads until breakline is reached
                while ((c = fgetc(instructions_file)) != EOF) // Instructions counting
                {                                             // read character by character and check if the end of the file is reached
                    if (c == '\n')
                    {
                        m.instructions_n++;
                    }
                }

                m.instructions_n++; // counter is increased by 1 since the last '\n' is lost. Placed here outside the if in case of just one instruction.
                if (m.instructions_n > 0)
                {
                    rewind(instructions_file); // rewinding file for instructions reading

                    instructions = (char **)calloc(m.instructions_n, sizeof(char *)); // allocating the char arrays array
                    if (m.m_mode == 0)
                    {
                        for (i = 0; i < m.instructions_n; i++)
                        {
                            instructions[i] = (char *)calloc(SINGLE_TAPE_RULES_NUMBER, sizeof(char)); // allocating the single char array, which is equal to an instruction
                        }
                    }
                    else
                    {
                        for (i = 0; i < m.instructions_n; i++)
                        {
                            instructions[i] = (char *)calloc(TWO_TAPES_RULES_NUMBER, sizeof(char)); // allocating the single char array, which is equal to an instruction
                        }
                    }

                    j = 1; // Using GP counter j for checking the pattern response from file content

                    if (m.m_mode == 0)
                    {
                        for (i = 0; i < m.instructions_n && j; i++) // filling instructions matrix for single tape machine
                        {
                            if (fscanf(instructions_file, "(%c, %c, %c, %c, %c)\n", &instructions[i][0], &instructions[i][1], &instructions[i][2], &instructions[i][3], &instructions[i][4]) != 5)
                            {
                                j = 0;
                            }
                        }
                    }
                    else
                    {
                        for (i = 0; i < m.instructions_n && j; i++) // filling instructions matrix for two tapes machine
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
                        m.status = '0';
                        m.tape_position = 0;
                        loaded = 1;                         // setting file check for other operations
                        copy_tape(m.input, m.backup_input); // backup of the tape
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
                print_tape(m.input);
                if (m.m_mode == 1) // If two tapes enable, also print the second tape
                    print_tape(second_tape);
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
                print_instructions(instructions, m.instructions_n, m.m_mode);
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
                print_machine_iteration(instructions[m.pos][0], m.tape_position, m.input); // First tape print. Useful if any instruction won't be chosen
                if (m.m_mode == 0)                                                         // If single mode chosen, use this algorithm
                {
                    do
                    {
                        m.pos = 0;
                        while (
                            (m.pos < m.instructions_n) &&
                            (m.tape_position < TAPE_DIM) &&
                            (instructions[m.pos][0] != m.status || instructions[m.pos][1] != m.input[m.tape_position]) &&
                            !(instructions[m.pos][0] == m.status && instructions[m.pos][1] == '-' && (m.input[m.tape_position] == '\0' || m.input[m.tape_position] == '*' || m.input[m.tape_position] == ' '))) // if looking for - on tape but reaching end of char array or space (human mane (' ') or machine made('*'))
                        {
                            m.pos++;
                        }

                        if (m.pos < m.instructions_n && m.tape_position < TAPE_DIM)
                        {
                            if (instructions[m.pos][2] == 'E')
                                check_end = 1; // if END reached, unflag check and exit loop
                            else
                                m.status = instructions[m.pos][2];

                            if (instructions[m.pos][3] == '-')
                                m.input[m.tape_position] = '*'; // I can understand if it is a machine made space
                            else
                                m.input[m.tape_position] = instructions[m.pos][3];

                            if (instructions[m.pos][4] == '>')
                                m.tape_position++;
                            else if (instructions[m.pos][4] == '<')
                                m.tape_position--;

                            print_machine_iteration(instructions[m.pos][0], m.tape_position, m.input);
                        }
                        else
                        {
                            check_end = 1; // Tape end reached or no instructions to execute
                        }
                    } while (m.pos < m.instructions_n && !check_end);
                }
                else
                { // Else (two tapes since the value of m.mode is strictly between 0 and 1), use this algorithm, which includes the additional tape. Another algorithm is required, since even the structure of instructions differs.
                }

                if (m.pos == m.instructions_n)
                {
                    print_no_instruction_found_alert();
                }

                m.pos = 0;                          // Restoring pos. Necessary for any outdated value from previous executions. If tape changes in size, then segfault would occur
                check_end = 0;                      // Restoring check end
                m.status = '0';                     // Restoring machine status
                m.tape_position = 0;                // Restoring tape position
                copy_tape(m.backup_input, m.input); // restoring the tape
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
                    if (m.m_mode == 1) // If the previous mode was two tapes, a reload of instructions is required
                        loaded = 0;
                    m.m_mode = 0;
                    print_mode_change_success("SINGLE");
                    if (second_tape != NULL)
                    {
                        free(second_tape); // freeing space allocated for the second tape
                    }
                }
                else if (user_choice[0] == '2')
                {
                    m.m_mode = 1;
                    print_mode_change_success("TWO");
                    second_tape = (char *)calloc(sizeof(char), TAPE_DIM); // Allocating for the second tape the same size of the primary tape. TAPE_DIM is chosen because the evolution of the tape caused by the program in unknown
                    loaded = 0;                                           // Refresh of loaded instructions required. To avoid that older single tape instructions could be processed as two tapes and generate false positives about syntax errors
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
