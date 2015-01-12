#include <stdio.h>
#include <stdlib.h>
#include "state_machine.h"

int main(void) {
	// Create our state machine
	StateMachine sm;

	// Initialize a state
	State *currentState = (State *)malloc(sizeof(State));
	currentState->status = STATE_OFF;
	currentState->handleState = PowerOn;

	// Set the state machine's current state
	sm.currentState = currentState;

	// Call handle state twice, one to transition out of
	// Power On and a second one to start the Idle loop
	sm.currentState->handleState(&sm);
	sm.currentState->handleState(&sm);

	return 0;
}