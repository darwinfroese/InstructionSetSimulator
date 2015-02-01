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

static int index = 0;
static int cmdOffset = 0;
static int argOneOffset = 0;
static int argTwoOffset = 0;
static int argThreeOffset = 0;

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
	else if (strcmp(cmd,"sub"))
		op = SUB;
	else if (strcmp(cmd,"mult"))
		op = MULT;
	else if (strcmp(cmd, "div"))
		op = DIV;
	else if (strcmp(cmd, "not"))
		op = NOT;
	else if (strcmp(cmd, "or"))
		op = OR;
	else if (strcmp(cmd, "and"))
		op = AND;
	else if (strcmp(cmd, "xor"))
		op = XOR;
	else if (strcmp(cmd, "jmp"))
		op = JMP;

	return op;
}

// if it's a value, return the value
// if it's a register, return the value in the register
unsigned char GetArg(char *arg) {
	if (arg[0] == REG_INDICATOR) {
		switch(arg[2]) {
		case 0:
			return currentRegisters->registerZero;
		case 1:
			return currentRegisters->registerOne;
		case 2:
			return currentRegisters->registerTwo;
		case 3:
			return currentRegisters->registerThree;
		case 4:
			return currentRegisters->registerFour;
		}
	}
	else {
		return (unsigned char)(atoi(arg));
	}

}

// Avoid code duplication by using this function to copy characters
void CopyCharacters(char term, int maxLen, int offset, char *dest, const char * const src) {
	while (src[index] != term && src[index] != ' ' && (index-offset) < (maxLen-1))	{
		dest[index-offset] = src[index];
		index++;
	}

	dest[index-offset] = '\0';
	index++;
}

// Returns the label (as a string) 
// if there is one at the start of the 
// line
char * GetLabel(const char * const line) {
	char label[LABEL_MAX];

	CopyCharacters('\t', LABEL_MAX, 0, label, line);
	cmdOffset = index;

	return label;
}

// Returns the command (as a char) so that 
// it is easier to use in other functions
unsigned char GetCommand(const char * const line) {
	unsigned char op;
	char cmd[CMD_LEN];

	CopyCharacters('\t', CMD_LEN, cmdOffset, cmd, line);
	argOneOffset = index;

	op = GetOp(cmd);

	return op;
}

// Get's the argument and stores it in the array 
// passed in.
void GetOneArgument(const char * const line, Command *cmd) {
	char arg[ARG_LEN];

	CopyCharacters(',', ARG_LEN, argOneOffset, arg, line);
	argTwoOffset = index;

	cmd->argOne = GetArg(arg);
}

// Calls the function to get the first argument and then
// grabs the second argument
void GetTwoArguments(const char * const line, Command *cmd) {
	char arg[ARG_LEN];

	GetOneArgument(line, cmd);

	CopyCharacters(',', ARG_LEN, argTwoOffset, arg, line);
	argThreeOffset = index;

	cmd->argTwo = GetArg(arg);
}

// Calls the function to get the first two arguments and then
// grabs the third argument
void GetThreeArguments(const char * const line, Command *cmd) {
	char arg[ARG_LEN];

	GetTwoArguments(line, cmd);

	CopyCharacters('\n', ARG_LEN, argThreeOffset, arg, line);
	
	cmd->argThree = GetArg(arg);
}

// Handles parsing the command based off what data is in
// each line
void ParseCommand(const char * const line, Command *c) {
	char label[LABEL_MAX];
	unsigned char cmd;
	unsigned char args[ARG_LEN];

	index = 0;

	// Check for a label
	if (line[index] != '\t') {		
		strcpy(label, GetLabel(line));
		c->label = label;
	}
	else {
		// Don't have label, need to set these up
		// to skip the opening tab
		cmdOffset = 1;
		index++;
	}

	cmd = GetCommand(line);
	c->op = cmd;

	// Get arguments
	if (cmd >= 10 && cmd <= 19) {
		GetOneArgument(line, c);
	}
	else if (cmd >= 20 && cmd <= 29) {
		GetTwoArguments(line, c);
	}
	else if (cmd >= 30 && cmd <= 39) {
		GetThreeArguments(line, c);
	}
}