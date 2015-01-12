#ifndef __CMD_INDEX_H
#define __CMD_INDEX_H

#include "state_machine.h"

// State Definitions -- These will be used to tell the current
// state which state to transition into
#define STOP	0
#define PAUSE	1
#define RESUME	2
#define EXIT	3

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