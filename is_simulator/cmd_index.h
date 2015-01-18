#ifndef __CMD_INDEX_H
#define __CMD_INDEX_H

#include "state_machine.h"

// Machine Operations
#define STOP	00
#define PAUSE	01
#define RESUME	02
#define EXIT	03
// 1 argument
#define BRANCH	10
// 2 arguments
#define NOT		20
// 3 arguments
#define ADD		30
#define SUB		31
#define MULT	32
#define DIV		33
#define OR		34
#define	AND		35
#define XOR		36

// Function Pointer for the Jump Table below
typedef void (*CommandHandle)(void *);

// Jump Table for the different command functions
// Each entry in the array contains a function for that command
CommandHandle commands[] = {
	Stop, 
	Pause,
	Resume,
	Exit
};

#endif