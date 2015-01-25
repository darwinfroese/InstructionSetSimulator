#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H

// States of the State Machine
#define STATE_OFF		-1
#define STATE_RUNNING	0
#define STATE_STOPPED	1
#define STATE_PAUSED	2
#define STATE_RESUME	3
#define STATE_EXITING	4

// A command object
typedef struct {
	unsigned char op;
	unsigned char argOne;	// destination
	unsigned char argTwo;	// source 1
	unsigned char argThree; // source 2
	char *label;
}Command;

// State object, contains the state and a function
// to process that state
typedef struct {
	int status;
	void (*handleState)(void *);
}State;

// State machine, primitive object that will be expanded 
// on as we add more features to the simulator
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