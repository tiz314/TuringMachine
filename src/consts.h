#include <stdio.h>

#define WAIT_TIME 10000
#define PROMPT "\n~$ > "

#define TAPE_DIM 50
#define SINGLE_TAPE_RULES_NUMBER 5
#define TWO_TAPES_RULES_NUMBER 8

#define TAPE_PATH "../input/tape.txt"
#define INSTR_PATH "../input/instructions.txt"

#ifdef __linux__ // including libraries for sleep, based on platform on which you compile
#include <unistd.h>
#else
#include <windows.h>
#endif

#ifndef __linux__ // defining name for __uint8_t for windows platform
typedef uint8_t __uint8_t
#endif
