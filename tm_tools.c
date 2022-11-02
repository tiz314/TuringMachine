#include <string.h>
#include <stdio.h>

#define MAX_INSTRUCTIONS 100
#define RULES_NUMBER 5
// | STATUS | INPUT | NEXT STATUS | OUTPUT | MOVEMENT | <-- Instructions scheme

struct tm_components
{
    char status;                                       // current machine status, starts from 0
    unsigned short int instructions_n;                 // number of received instructions
    unsigned short int tape_position;                  // tape position = position in "input" array
    unsigned short int pos;                            // positive counter for the position in the instruction matrix
    char instructions[MAX_INSTRUCTIONS][RULES_NUMBER]; // TM instructions matrix, TODO: CHANGE
};