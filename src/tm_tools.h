#include "consts.h"
#include "tm.h"
#include <string.h>

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