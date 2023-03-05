#include "./headers/consts.h"
#include "./headers/tm_tools.h" // necessary to find typedef in header file

/*
The instruction schema used for instructions depends on the two main modes of operation of the machine: one or two tapes:

One tape version:       | STATUS | INPUT | NEXT STATUS | OUTPUT | HEAD MOVEMENT |
Two tapes version:      | STATUS | INPUT TAPE 1 | INPUT TAPE 2 | NEXT STATUS | OUTPUT TAPE 1 | OUTPUT TAPE 2 | HEAD 1 MOVEMENT | HEAD 2 MOVEMENT |

*/

void init_tape(cell *c)
{ // remember to send the first element with prev as NULL!!
    for (cell *cp = c; cp != NULL; cp = (cell *)cp->next)
    {
        cp->element = '\0';
    }
}

void print_cell_sequence(cell *c) // Only for debugging!
{
    for (cell *cp = c; cp != NULL; cp = (cell *)cp->next)
    {
        printf("%p, %p, %p: %d\n", cp->prev, cp, cp->next, cp->element);
    }
    puts("");
}

void copy_tape(cell *orig, cell *dest)
{
    cell *dp = dest;
    for (cell *op = orig; op->next != NULL; op = (cell *)op->next)
    {
        dp->element = op->element;
        if (dp->next == NULL)
        {
            dp->next = (struct cell *)calloc(1, sizeof(cell));
        }
        dp = (cell *)dp->next;
        dp->element = 0;
    }
}

cell *get_first_element(cell *c)
{
    cell *cp;
    for (cp = c; cp->prev != NULL; cp = (cell *)cp->prev)
        ;
    return cp;
}

void fill_tape(FILE *input_file, cell *c)
{ // passing the first cell ever allocated, with prev NULL
    char from_file;
    cell *cp = c, *swap_cp;
    while (fscanf(input_file, "%c", &from_file) != EOF)
    {
        if (from_file == ' ')
            cp->element = '\0';
        else
            cp->element = from_file;

        if (cp->next == NULL)
        {
            cp->next = (struct cell *)calloc(1, sizeof(cell));
        }
        swap_cp = cp;
        cp = (cell *)cp->next;
        cp->element = 0;
        cp->prev = (struct cell *)swap_cp;
    }
}

void free_tape(cell *c)
{
    cell *cp = (cell *)c->next, *temp_cp;
    while (cp != NULL)
    {
        temp_cp = (cell *)cp->next;
        free(cp);
        cp = (cell *)temp_cp;
    }
    free(c);
}