#include <stdlib.h>
#include "state_machine.h"
#include "cmd_index.h"

static StateMachine *stateMachine;

Command *GetCommand() {
	Command *c = (Command *)malloc(sizeof(Command));
	c->op = STOP;
	c->argOne = -1;
	c->argTwo = -1;
	c->argThree = -1;

	if (stateMachine->currentState->status == STATE_STOPPED)
		c->op = EXIT;

	return c;
}

void ProcessStateChange(int nextState, State *s) {
	switch(nextState) {
	case STATE_RUNNING: 
		s->status = nextState;
		s->handleState = Idle;
		break;
	case STATE_STOPPED:
		s->status = nextState;
		break;
	case STATE_PAUSED:
		s->status = nextState;
		break;
	case STATE_RESUME:
		s->status = nextState;
		break;
	case STATE_EXITING:
		s->status = nextState;
		break;
	}
}

void Idle(void *d) {
	while(stateMachine->currentState->status != STATE_EXITING) {
		// get command
		Command *c = GetCommand();
		// call command function
		commands[c->op](NULL);
	}
}

void PowerOn(void *d) {
	stateMachine = (StateMachine *)d;
	ProcessStateChange(STATE_RUNNING, stateMachine->currentState);
}

void Stop(void *d) {
	ProcessStateChange(STATE_STOPPED, stateMachine->currentState);
}

void Pause(void *d) {
	ProcessStateChange(STATE_PAUSED, stateMachine->currentState);
}

void Resume(void *d) {
	ProcessStateChange(STATE_RESUME, stateMachine->currentState);
}

void Exit(void *d) {
	ProcessStateChange(STATE_EXITING, stateMachine->currentState);
}