#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H

// States
#define STATE_OFF		-1
#define STATE_RUNNING	0
#define STATE_STOPPED	1
#define STATE_PAUSED	2
#define STATE_RESUME	3
#define STATE_EXITING	4

// Change states
// current state -- how to process
typedef struct {
	unsigned char op;
	unsigned char argOne;
	unsigned char argTwo;
	unsigned char argThree;
}Command;

typedef struct {
	int status;
	void (*handleState)(void *);
}State;

typedef struct {
	State *currentState;
}StateMachine;

// Function Declerations
void Idle(void *d);
void PowerOn(void *d);
void Stop(void *d);
void Pause(void *d);
void Resume(void *d);
void Exit(void *d);


#endif