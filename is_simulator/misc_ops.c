#include <stdio.h>
#include "registers.h"
#include "operations.h"
#include "cmd_index.h"

void PrintRegisters(void *data) {
	fprintf(stdout, "Register Values\n");
	fprintf(stdout, "--------------------------------\n");
	fprintf(stdout, "$r0 : %d\n", currentRegisters->registerZero);
	fprintf(stdout, "$r1 : %d\n", currentRegisters->registerOne);
	fprintf(stdout, "$r2 : %d\n", currentRegisters->registerTwo);
	fprintf(stdout, "$r3 : %d\n", currentRegisters->registerThree);
	fprintf(stdout, "$r4 : %d\n", currentRegisters->registerFour);
	fprintf(stdout, "--------------------------------\n\n");
}

void NotImplemented(void *data) {
	fprintf(stderr, "No operation implemented yet.\n");
}