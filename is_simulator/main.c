#include <stdio.h>
#include <stdlib.h>
#include "state_machine.h"

int main(void) {
	// Create our state machine
	StateMachine sm;

	// Initialize the state machine
	State *currentState = (State *)malloc(sizeof(State));
	currentState->status = STATE_OFF;
	currentState->handleState = PowerOn;

	sm.currentState = currentState;
	sm.currentState->handleState(&sm);
	sm.currentState->handleState(&sm);

	return 0;
}