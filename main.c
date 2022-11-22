#include <stdio.h>
#include <stdlib.h>

#include "tm_tools.c"
#include "interface_supp.c"

#define TAPE_PATH "input/tape.txt"
#define INSTR_PATH "input/instructions.txt"

int main(int argc, char const *argv[])
{
    __uint8_t check_end, check_menu = 1, loaded = 0, i, j; // check if END command reached; check for the menu cycle; check for file loading; general purpose counters
    char user_choice[BUFSIZ];                              // user input in the menu
    struct tm_components m;
    char **instructions; // char pointer for dynamic matrix of instructions

    char c;           // char use to read single chars from files
    FILE *input_file; // opens read only input.txt and instructions.txt
    FILE *instructions_file;

    print_boot();

    while (check_menu)
    {
        print_menu();
        scanf("\n%s", user_choice);

        switch (user_choice[0])
        {

        case '1':
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
                    for (i = 0; i < m.instructions_n; i++)
                    {
                        instructions[i] = (char *)calloc(RULES_NUMBER, sizeof(char)); // allocating the single char array, which is equal to an instruction
                    }

                    j = 1;                                          // Using GP counter j for checking the pattern response from file content
                    for (int i = 0; i < m.instructions_n && j; i++) // filling instructions matrix
                    {
                        if (fscanf(instructions_file, "(%c, %c, %c, %c, %c)\n", &instructions[i][0], &instructions[i][1], &instructions[i][2], &instructions[i][3], &instructions[i][4]) != 5)
                        {
                            j = 0;
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
            break;
        }

        case '2':
        {
            if (loaded)
            {
                print_tape(m.input);
            }
            else
            {
                print_loading_warning();
            }
            break;
        }

        case '3':
        {
            if (loaded)
            {
                print_instructions(instructions, m.instructions_n);
            }
            else
            {
                print_loading_warning();
            }
            break;
        }

        case '4':
        {
            if (loaded)
            {
                m.pos = 0;                                                                 // Necessary for any outdated value from previous executions. If tape changes in size, then segfault would occur
                print_machine_iteration(instructions[m.pos][0], m.tape_position, m.input); // First tape print. Useful if any instruction won't be chosen
                do
                {
                    m.pos = 0;
                    while (
                        (m.pos < m.instructions_n) &&
                        (instructions[m.pos][0] != m.status || instructions[m.pos][1] != m.input[m.tape_position]) &&
                        !(instructions[m.pos][0] == m.status && instructions[m.pos][1] == '-' && (m.input[m.tape_position] == '\0' || m.input[m.tape_position] == '*' || m.input[m.tape_position] == ' '))) // if looking for - on tape but reaching end of char array or space (human mane (' ') or machine made('*'))
                    {
                        m.pos++;
                    }

                    if (m.pos < m.instructions_n)
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
                } while (m.pos < m.instructions_n && !check_end);

                if (m.pos == m.instructions_n)
                {
                    print_no_instruction_found_alert();
                }

                check_end = 0;                      // Restoring check end
                m.status = '0';                     // Restoring machine status
                m.tape_position = 0;                // Restoring tape position
                copy_tape(m.backup_input, m.input); // restoring the tape
            }
            else
            {
                print_loading_warning();
            }
            break;
        }

        case '5':
        {
            print_help();
            break;
        }

        case '6':
        {
            print_about();
            break;
        }

        case '7':
        {
            if (instructions_file)
                fclose(instructions_file);
            if (input_file)
                fclose(input_file); // Cosing files

            print_exiting();
            check_menu = 0;
            break;
        }
        }
    }

    return 0;
}
