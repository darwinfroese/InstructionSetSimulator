#include <stdlib.h>
#include "registers.h"
#include "file_reader.h"
#include "state_machine.h"
#include "operations.h"
#include "cmd_index.h"
#include "cmd_parser.h"

// This is the local state machine so that it can be
// accessed when states change easily
static StateMachine *stateMachine;
RegisterState *currentRegisters;
static void *DATA = NULL;

// Build a data block that we can send to our command
// function. Use switch case instead of if/else so that
// we can add functionality to individual numbers if we
// have the need to later
void CreateDataBlock(Command *c) {
	OneArg_s *arg;
	TwoArgs_s *args2;
	ThreeArgs_s *args3;

	switch(c->op) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		DATA = NULL;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:	
		arg = (OneArg_s *)malloc(sizeof(OneArg_s));
		arg->argOne = c->argOne;
		DATA = arg;
		break;
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
		args2 = (TwoArgs_s *)malloc(sizeof(TwoArgs_s));
		args2->argOne = c->argOne;
		args2->argTwo = c->argTwo;
		DATA = args2;
		break;
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
		args3 = (ThreeArgs_s *)malloc(sizeof(ThreeArgs_s));
		args3->argOne = c->argOne;
		args3->argTwo = c->argTwo;
		args3->argThree = c->argThree;
		DATA = args3;
		break;
	}
}

// This reads the next command from the input file
// Currently - Static processing
Command *GetCommandInformation() {
	char *line = (char *)malloc(LINE_MAX);
	Command *c = (Command *)malloc(sizeof(Command));
	
	line = ParseFile();

	// Decompose the command
	ParseCommand(line, c);
	CreateDataBlock(c);

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
		Command *c = GetCommandInformation();
		// call command function
		commands[c->op](DATA);
	}
}

// Switches out of the PowerOn state
void PowerOn(void *d) {
	stateMachine = (StateMachine *)d;
	// Init registers
	currentRegisters = (RegisterState *)malloc(sizeof(RegisterState));
	currentRegisters->programCounter = 0;
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