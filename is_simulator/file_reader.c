#include "file_reader.h"

#include <stdio.h>
#include <stdlib.h>

#define PATH_MAX			1024
#define FILE_MODE			0
#define INTERACTIVE_MODE	1

static FILE *inputFile = NULL;
static int mode_s;

char * GetFileLocation() {
	char path[PATH_MAX];
	fprintf(stdout, "Enter path to file: ");

	fgets(path, PATH_MAX, stdin);

	return path;
}

// Allows us to open the simulator in an interactive mode
// where we enter commands one at a time and they are parsed or
// acted on immediately or specify a file that we want to read
// from
void OpenFile() {
	char *path = GetFileLocation();
	mode_s = FILE_MODE;

	// interactive or file
	if (path[0] == '\n')
		mode_s = INTERACTIVE_MODE;
	else
		inputFile = fopen(path, "rb");
}

// Reads the line from the file or command line
// based on what mode we are in.
char * ParseFile() {
	char line[LINE_MAX];

	if (inputFile == NULL && mode_s == FILE_MODE) {
		fprintf(stderr, "File was not opened.\n");
		exit(-1);
	}
	
	switch(mode_s) {
	case FILE_MODE:
		fgets(line, LINE_MAX, inputFile);
		break;
	case INTERACTIVE_MODE:
		fgets(line, LINE_MAX, stdin);
		break;
	}

	return line;
}