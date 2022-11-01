#include <stdio.h>
#include "tm_tools.c"

int main(int argc, char const *argv[])
{
    char input[BUFSIZ];  // TM tape; DEBUG INPUT
    __uint8_t check = 1; // check if END command reached
    struct tm_components m;
    m.instructions_n = 0; // used later to count instructions in file

    // ---- loading from file procedure ----
    char c;                                     // char use to read single chars from files
    FILE *input_file = fopen("input.txt", "r"); // opens read only input.txt and instructions.txt
    FILE *instructions_file = fopen("instructions.txt", "r");

    if (instructions_file == NULL || input_file == NULL)
    { // checks if input files exist
        printf("Input or instructions file not found\n");
        return 0;
    }

    fscanf(input_file, "%[^\n]", input); // reads until breakline is reached

    while ((c = fgetc(instructions_file)) != EOF)
    { // read character by character and check if the end of the file is reached
        if (c == '\n')
        {
            m.instructions_n++;
        }
    }
    if (m.instructions_n == 0)
    {
        printf("No instruction given\n");
        return 0;
    }

    // initializing struct variables
    m.status = '0';
    m.instructions_n++; // m.instructions is increased by 1 since the last '\n' is lost
    m.tape_position = 0;
    printf("ISTRUZIONI %d\n\n", m.instructions_n);

    char instructions[m.instructions_n][RULES_NUMBER]; // TM instructions matrix

    instructions_file = fopen("instructions.txt", "r");

    for (int i = 0; i < m.instructions_n; i++)
    { // fill instructions matrix
        fscanf(instructions_file, "(%c, %c, %c, %c, %c)\n", &instructions[i][0], &instructions[i][1], &instructions[i][2], &instructions[i][3], &instructions[i][4]);
    }
    // ---- end of loading from file procedure ----

    print_tape(input);

    int i, j; // DEBUG ONLY
    for (i = 0; i < m.instructions_n; i++)
    {
        for (j = 0; j < RULES_NUMBER; j++)
        {
            printf("%c\t", instructions[i][j]);
        }
        printf("\n");
    }

    do
    {
        m.pos = 0;
        while (
            (instructions[m.pos][0] != m.status || instructions[m.pos][1] != input[m.tape_position]) &&
            m.pos < m.instructions_n &&
            !(instructions[m.pos][0] == m.status && instructions[m.pos][1] == '-' && (input[m.tape_position] == '\0' || input[m.tape_position] == '*' || input[m.tape_position] == ' '))) // if looking for - on tape but reaching end of char array or space (already there or machine made)
        {
            m.pos++;
        }

        if (m.pos < m.instructions_n)
        {
            if (instructions[m.pos][2] == 'E')
                check = 0; // if END reached, unflag check and exit loop
            else
                m.status = instructions[m.pos][2];

            if (instructions[m.pos][3] == '-')
                input[m.tape_position] = '*'; // I can understand if it is a machine made space
            else
                input[m.tape_position] = instructions[m.pos][3];

            if (instructions[m.pos][4] == '>')
                m.tape_position++;
            else if (instructions[m.pos][4] == '<')
                m.tape_position--;
        }
        printf("Pos: %d\tStatus: %c\tTape position: %d; ", m.pos, instructions[m.pos][0], m.tape_position);
        printf("Result:\t%s\n", input);
    } while (m.pos < m.instructions_n && check);

    printf("Result:\t");
    print_tape(input);
    return 0;
}
