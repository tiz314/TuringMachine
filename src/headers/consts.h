#include <stdio.h>
#include <stdlib.h>

#define WAIT_TIME 10000
#define PROMPT "\n~$ > "

#define SINGLE_TAPE_RULES_NUMBER 5
#define TWO_TAPES_RULES_NUMBER 8

#define TAPE_PATH "../input/tape.txt"
#define INSTR_PATH "../input/instructions.txt"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
typedef u_int8_t __u_int8_t;
#else
#include <unistd.h>
#endif