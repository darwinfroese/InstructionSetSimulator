#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cmd_parser.h"
#include "cmd_index.h"
#include "reg_index.h"

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

	if (strcmp(cmd,"add"))
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
unsigned char GetArg(char *args, unsigned char *arg) {
	unsigned char arg_type;

	if (args[0] == REG_INDICATOR) {
		arg_type = 'r';

		switch (args[2]) {
		case 0:
			*arg = REG_0;
			break;
		case 1:
			*arg = REG_1;
			break;
		case 2:
			*arg = REG_2;
			break;
		case 3:
			*arg = REG_3;
			break;
		case 4:
			*arg = REG_4;
			break;
		}
	}
	else {
		arg_type = 'v';
		*arg = (unsigned char)atoi(args);
	}

	return arg_type;
}

// Avoid code duplication by using this function to copy characters
void CopyCharacters(char term, int maxLen, int offset, char *dest, char *src) {
	while (src[index] != '\t' || (index-offset) < (maxLen-1))	{
		dest[index-offset] = src[index];
		index++;
	}

	dest[index-offset] = '\0';
	index++;
}

// Returns the label (as a string) 
// if there is one at the start of the 
// line
char * GetLabel(char *line) {
	char *label = (char *)malloc(LABEL_MAX);

	CopyCharacters('\t', LABEL_MAX, 0, label, line);
	cmdOffset = index;

	return label;
}

// Returns the command (as a char) so that 
// it is easier to use in other functions
unsigned char GetCommand(char *line) {
	unsigned char op;
	char cmd[CMD_LEN];

	CopyCharacters('\t', CMD_LEN, cmdOffset, cmd, line);
	argOneOffset = index;

	op = GetOp(cmd);

	return op;
}

// Get's the argument and stores it in the array 
// passed in.
char GetOneArgument(char *line, unsigned char *args) {
	char arg[ARG_LEN];

	CopyCharacters(',', ARG_LEN, argOneOffset, arg, line);
	argTwoOffset = index;

	return GetArg(arg, &args[0]);
}

// Calls the function to get the first argument and then
// grabs the second argument
char GetTwoArguments(char *line, unsigned char *args) {
	char arg[ARG_LEN];

	GetOneArgument(line, args);

	CopyCharacters(',', ARG_LEN, argTwoOffset, arg, line);
	argThreeOffset = index;

	return GetArg(arg, &args[1]);
}

// Calls the function to get the first two arguments and then
// grabs the third argument
char GetThreeArguments(char *line, unsigned char *args) {
	char arg[ARG_LEN];

	GetTwoArguments(line, args);

	CopyCharacters(',', ARG_LEN, argThreeOffset, arg, line);

	return GetArg(arg, &args[2]);
}

// Handles parsing the command based off what data is in
// each line
void ParseCommand(char *line, Command *c) {
	char *label;
	unsigned char cmd;
	unsigned char *args = (unsigned char *)malloc(3);

	index = 0;

	// Check for a label
	if (line[index] != '\t') {
		label = GetLabel(line);
		c->label = label;
	}

	cmd = GetCommand(line);
	c->op = cmd;

	// Get arguments
	if (cmd >= 10 && cmd <= 19) {
		if (GetOneArgument(line, args) == 'r')
			c->argOne_r = args[0];
		else
			c->argOne_v = args[0];
	}
	else if (cmd >= 20 && cmd <= 29) {
		if (GetTwoArguments(line, args) =='r') {
			c->argOne_r = args[0];
			c->argTwo_r = args[1];
		}
		else {
			c->argOne_v = args[0];
			c->argTwo_v = args[1];
		}
	}
	else if (cmd >= 30 && cmd <= 39) {
		GetThreeArguments(line, args);
		c->argOne_r = args[0];
		c->argTwo_r = args[1];
		c->argThree = args[2];
	}
}