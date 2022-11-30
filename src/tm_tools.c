#include "./headers/consts.h"

/*
The instruction schema used for instructions depends on the two main modes of operation of the machine: one or two tapes:

One tape version:       | STATUS | INPUT | NEXT STATUS | OUTPUT | HEAD MOVEMENT |
Two tapes version:      | STATUS | INPUT TAPE 1 | INPUT TAPE 2 | NEXT STATUS | OUTPUT TAPE 1 | OUTPUT TAPE 2 | HEAD 1 MOVEMENT | HEAD 2 MOVEMENT |

*/

void init_tape(char *tape)
{
    for (int i = 0; i < TAPE_DIM; i++)
    {
        tape[i] = ' ';
    }
}

void copy_tape(char *orig, char *dest)
{ // used for backupping the tape and restoring it
    for (int i = 0; i < TAPE_DIM; i++)
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
