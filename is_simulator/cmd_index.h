#ifndef __CMD_INDEX_H
#define __CMD_INDEX_H

#include "state_machine.h"

#define STOP	0
#define PAUSE	1
#define RESUME	2
#define EXIT	3

typedef void (*CommandHandle)(void *);

CommandHandle commands[] = {
	Stop,
	Pause,
	Resume,
	Exit
};

#endif