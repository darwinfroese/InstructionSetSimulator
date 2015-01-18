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

void OpenFile() {
	char *path = GetFileLocation();
	mode_s = FILE_MODE;

	// interactive or file
	if (path[0] == '\n')
		mode_s = INTERACTIVE_MODE;
	else
		inputFile = fopen(path, "rb");
}

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