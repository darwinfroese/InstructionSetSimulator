#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cmd_parser.h"
#include "cmd_index.h"
#include "reg_index.h"
#include "registers.h"
#include "operations.h"

#define LABEL_MAX		256
#define CMD_LEN			5
#define ARG_LEN			4
#define REG_INDICATOR	'$'
#define LINE_LEN		1024
#define REG_LOC			0
#define REG_VAL			1

static int index = 0;
static int cmdOffset = 0;
static int argOneOffset = 0;
static int argTwoOffset = 0;
static int argThreeOffset = 0;
static char *READ_LINE;

// Get the operation that we want as an unsigned char
// instead of a string so that we can use it in our 
// other functions easier
unsigned char GetOp(char *cmd) {
	int i;
	unsigned char op;

	for(i = 0; i<CMD_LEN-1; i++) {
		cmd[i] = tolower(cmd[i]);
	}

	if (strcmp(cmd,"add") == 0)
		op = ADD;
	else if (strcmp(cmd,"sub") == 0)
		op = SUB;
	else if (strcmp(cmd,"mult") == 0)
		op = MULT;
	else if (strcmp(cmd, "div") == 0)
		op = DIV;
	else if (strcmp(cmd, "not") == 0)
		op = NOT;
	else if (strcmp(cmd, "or") == 0)
		op = OR;
	else if (strcmp(cmd, "and") == 0)
		op = AND;
	else if (strcmp(cmd, "xor") == 0)
		op = XOR;
	else if (strcmp(cmd, "jmp") == 0)
		op = JMP;
	else if (strcmp(cmd, "prv") == 0)
		op = PRV;

	return op;
}

// if it's a value, return the value
// if it's a register, return the value in the register
unsigned char GetArg(char *arg, int flag) {
	unsigned char retVal;

	if (arg[0] == REG_INDICATOR) {
		switch(arg[2]) {
		case '0':
			retVal = (flag == REG_LOC ? 0 : currentRegisters->registerZero);
			break;
		case '1':
			retVal = (flag == REG_LOC ? 1 : currentRegisters->registerOne);
			break;
		case '2':
			retVal = (flag == REG_LOC ? 2 : currentRegisters->registerTwo);
			break;
		case '3':
			retVal = (flag == REG_LOC ? 3 : currentRegisters->registerThree);
			break;
		case '4':
			retVal = (flag == REG_LOC ? 4 : currentRegisters->registerFour);
			break;
		}
	}
	else {
		return (unsigned char)(atoi(arg));
	}

	return retVal;
}

// Avoid code duplication by using this function to copy characters
void CopyCharacters(char term, int maxLen, int offset, char *dest) {
	while (READ_LINE[index] != term && READ_LINE[index] != ' ' 
		&& READ_LINE[index] != '\n' && (index-offset) < (maxLen-1))	{
		dest[index-offset] = READ_LINE[index];
		index++;
	}

	dest[index-offset] = '\0';
	index++;
}

// Returns the label (as a string) 
// if there is one at the start of the 
// line
char * GetLabel() {
	char label[LABEL_MAX];

	CopyCharacters('\t', LABEL_MAX, 0, label);
	cmdOffset = index;

	return label;
}

// Returns the command (as a char) so that 
// it is easier to use in other functions
unsigned char GetCommand() {
	unsigned char op;
	char cmd[CMD_LEN];

	CopyCharacters('\t', CMD_LEN, cmdOffset, cmd);
	argOneOffset = index;

	op = GetOp(cmd);

	return op;
}

// Get's the argument and stores it in the array 
// passed in.
void GetOneArgument(Command *cmd) {
	char arg[ARG_LEN];

	CopyCharacters(',', ARG_LEN, argOneOffset, arg);
	argTwoOffset = index;

	if (cmd->op >= 20)
		cmd->argOne = GetArg(arg, REG_LOC);
	else
		cmd->argOne = GetArg(arg, REG_VAL);
}

// Calls the function to get the first argument and then
// grabs the second argument
void GetTwoArguments(Command *cmd) {
	char arg[ARG_LEN];

	GetOneArgument(cmd);

	CopyCharacters(',', ARG_LEN, argTwoOffset, arg);
	argThreeOffset = index;

	cmd->argTwo = GetArg(arg, REG_VAL);
}

// Calls the function to get the first two arguments and then
// grabs the third argument
void GetThreeArguments(Command *cmd) {
	char arg[ARG_LEN];

	GetTwoArguments(cmd);

	CopyCharacters('\n', ARG_LEN, argThreeOffset, arg);
	
	cmd->argThree = GetArg(arg, REG_VAL);
}

// Handles parsing the command based off what data is in
// each line
void ParseCommand(const char * const line, Command *c) {
	char label[LABEL_MAX];
	unsigned char cmd;
	unsigned char args[ARG_LEN];

	index = 0;
	READ_LINE = (char *)malloc(LINE_LEN);
	strcpy(READ_LINE, line);

	// Check for a label
	if (line[index] != '\t') {		
		strcpy(label, GetLabel());
		c->label = label;
	}
	else {
		// Don't have label, need to set these up
		// to skip the opening tab
		cmdOffset = 1;
		index++;
	}

	cmd = GetCommand();
	c->op = cmd;

	// Get arguments
	if (cmd >= 10 && cmd <= 19) {
		GetOneArgument(c);
	}
	else if (cmd >= 20 && cmd <= 29) {
		GetTwoArguments(c);
	}
	else if (cmd >= 30 && cmd <= 39) {
		GetThreeArguments(c);
	}
}