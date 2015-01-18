#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cmd_parser.h"
#include "cmd_index.h"
#include "reg_index.h"

#define LABEL_MAX	256
#define CMD_LEN		5
#define ARG_LEN		4

static int index = 0;
static int cmdOffset = 0;
static int argOneOffset = 0;
static int argTwoOffset = 0;
static int argThreeOffset = 0;

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
	else if (strcmp(cmd, "br"))
		op = BRANCH;

	return op;
}

unsigned char GetArg(char *args) {
	// Get register or value
}

char * GetLabel(char *line) {
	char *label = (char *)malloc(LABEL_MAX);

	while (line[index] != '\t' || index < (LABEL_MAX-1))	{
		label[index] = line[index];
		index++;
	}

	label[index] = '\0';
	index++;
	cmdOffset = index;

	return label;
}

unsigned char GetCommand(char *line) {
	unsigned char op;
	char cmd[CMD_LEN];

	while (line[index] != '\t' || (index-cmdOffset) < (CMD_LEN-1)) {
		cmd[index-cmdOffset] = line[index];
		index++;
	}

	cmd[index-cmdOffset] = '\0';
	index++;
	argOneOffset = index;

	op = GetOp(cmd);

	return op;
}

void GetOneArgument(char *line, unsigned char *args) {
	char arg[ARG_LEN];

	while (line[index] != ',' || (index-argOneOffset) < (ARG_LEN-1)) {
		arg[index-argOneOffset] = line[index];
		index++;
	}

	arg[index-argOneOffset] = '\0';
	index++;
	argTwoOffset = index;

	args[0] = GetArg(arg);
}

void GetTwoArguments(char *line, unsigned char *args) {
	char arg[ARG_LEN];

	GetOneArgument(line, args);

	while (line[index] != ',' || (index-argTwoOffset) < (ARG_LEN-1)) {
		arg[index-argTwoOffset] = line[index];
		index++;
	}

	arg[index-argTwoOffset] = '\0';
	index++;
	argThreeOffset = index;

	args[1] = GetArg(arg);
}

void GetThreeArguments(char *line, unsigned char *args) {
	char arg[ARG_LEN];

	GetTwoArguments(line, args);

	while (line[index] != ',' || (index-argThreeOffset) < (ARG_LEN-1)) {
		arg[index-argThreeOffset] = line[index];
		index++;
	}

	arg[index-argThreeOffset] = '\0';

	args[2] = GetArg(arg);
}

void ParseCommand(char *line, Command *c) {
	char *label;
	unsigned char cmd;
	unsigned char *args = (unsigned char *)malloc(3);

	// Check for a label
	if (line[0] != '\t') {
		label = GetLabel(line);
		c->label = label;
	}
	else {
		cmd = GetCommand(line);
		c->op = cmd;
	}	

	// Get arguments
	if (cmd >= 10 && cmd <= 19) {
		GetOneArgument(line, args);
		c->argOne = args[0];
	}
	else if (cmd >= 20 && cmd <= 29) {
		GetTwoArguments(line, args);
		c->argOne = args[0];
		c->argTwo = args[1];
	}
	else if (cmd >= 30 && cmd <= 39) {
		GetThreeArguments(line, args);
		c->argOne = args[0];
		c->argTwo = args[1];
		c->argThree = args[2];
	}
}