#include <string.h>
#include <stdio.h>

#ifndef __linux__ // defining name for __uint8_t for windows platform
typedef uint8_t __uint8_t
#endif

#define MAX_INSTRUCTIONS 100
#define RULES_NUMBER 5

    /*
    The instruction schema used for instructions depends on the two main modes of operation of the machine: one or two tapes:

    One tape version:       | STATUS | INPUT | NEXT STATUS | OUTPUT | HEAD MOVEMENT |
    Two tapes version:      | STATUS | INPUT TAPE 1 | INPUT TAPE 2 | NEXT STATUS | OUTPUT TAPE 1 | OUTPUT TAPE 2 | HEAD 1 MOVEMENT | HEAD 2 MOVEMENT |

    */
    typedef struct
{
    __uint8_t m_mode;                  // Machine mode of operation: it can be 0 (one tape machine) or 1 (two tapes machine)
    char status;                       // current machine status, starts from 0
    unsigned short int instructions_n; // number of received instructions
    unsigned short int tape_position;  // tape position = position in "input" array
    unsigned short int pos;            // positive counter for the position in the instruction matrix
    char input[BUFSIZ];                // TM tape. Not dinamically allocated because of the possibility to write new slots by the program itself
    char backup_input[BUFSIZ];         // backup char array. The original tape is copied also here, in order to recover it without realoding from file.
} tm_components;

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


/*void set_default_settings(tm_components *m)
{                  // In case of errors while loading config file, default settings are provided
    m->m_mode = 0; // Mode of operation is set to single tape mode
    // Update if new settings introduced
}

int load_settings(FILE *file, tm_components *m)
{
    __uint8_t success = 1, mode;
    if (scanf("tmmode: %hhd", &mode) != 1)
        success = 0;

    // For possible implementation of new settings in the future.

    if (success)
        return 0;
    else
        return -1;
}*/