#include <stdlib.h>
#include "file_reader.h"
#include "state_machine.h"
#include "cmd_index.h"
#include "cmd_parser.h"

// This is the local state machine so that it can be
// accessed when states change easily
static StateMachine *stateMachine;

// This reads the next command from the input file
// Currently - Static processing
Command *GetCommand() {
	char *line = (char *)malloc(LINE_MAX);
	Command *c = (Command *)malloc(sizeof(Command));
	
	line = ParseFile();

	// Decompose the command
	ParseCommand(line, c);

	return c;
}

// Changes the states based on the input, sets
// the function if necessary
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

// The Idle loop, this is where most of the commands will get processed from
// This is the main loop of our simulator
void Idle(void *d) {
	while(stateMachine->currentState->status != STATE_EXITING) {
		// get command
		Command *c = GetCommand();
		// call command function
		commands[c->op](NULL);
	}
}

// Switches out of the PowerOn state
void PowerOn(void *d) {
	stateMachine = (StateMachine *)d;

	// Open the file
	OpenFile();

	ProcessStateChange(STATE_RUNNING, stateMachine->currentState);
}

// Switches into a stopped state
void Stop(void *d) {
	ProcessStateChange(STATE_STOPPED, stateMachine->currentState);
}

// Switches into a paused state
void Pause(void *d) {
	ProcessStateChange(STATE_PAUSED, stateMachine->currentState);
}

// Resumes from pause *only*
void Resume(void *d) {
	ProcessStateChange(STATE_RESUME, stateMachine->currentState);
}

// Exits the state machine
void Exit(void *d) {
	ProcessStateChange(STATE_EXITING, stateMachine->currentState);
}