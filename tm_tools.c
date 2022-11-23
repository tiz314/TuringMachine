#include <string.h>
#include <stdio.h>

#ifndef __linux__ // defining name for __uint8_t for windows platform
typedef uint8_t __uint8_t
#endif

#define MAX_INSTRUCTIONS 100
#define RULES_NUMBER 5

// | STATUS | INPUT | NEXT STATUS | OUTPUT | MOVEMENT | <-- Instructions schema
struct tm_components
{
    char status;                       // current machine status, starts from 0
    unsigned short int instructions_n; // number of received instructions
    unsigned short int tape_position;  // tape position = position in "input" array
    unsigned short int pos;            // positive counter for the position in the instruction matrix
    char input[BUFSIZ];                // TM tape. Not dinamically allocated because of the possibility to write new slots by the program itself
    char backup_input[BUFSIZ];         // backup char array. The original tape is copied also here, in order to recover it without realoding from file.
};

void init_tape(char *tape)
{
    int i;
    for (i = 0; tape[i] != '\0'; i++)
    {
        tape[i] = ' ';
    }
}

void copy_tape(char *orig, char *dest)
{ // used for backupping the tape and restoring it
    int i;
    for (i = 0; i < BUFSIZ; i++)
    {
        dest[i] = orig[i];
    }
}