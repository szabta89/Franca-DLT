#include <stdlib.h>
#include <stdio.h>
#include <dlt/dlt.h>
#include <string.h>

DLT_DECLARE_CONTEXT(context);

int main(int argc, char** argv) {
	if (argc < 4) {
		printf("Usage: tracegen APPID CONTEXTID path_to_trace_file [delay]\n");
		return 1;
	}
	else {
		char* appid = argv[1];
		char* contextid = argv[2];
		char* path = argv[3];

		int delay = 1;
		if (argc == 5) {
			delay = atoi(argv[4]);
		}

		DLT_REGISTER_APP(appid, "Franca test application");
		DLT_REGISTER_CONTEXT(context, contextid, "Franca test context");

		FILE * fp;
		char* line = NULL;
		size_t len = 0;
		ssize_t read;

		fp = fopen(path, "r");
		if (fp == NULL) {
			printf("The specified file cannot be found or cannot be opened!\n");
			return 1;
		}

		while ((read = getline(&line, &len, fp)) != -1) {
			if (line[strlen(line) - 1] == '\n') {
				line[strlen(line) - 1] = 0;
			}
			DLT_LOG(context, DLT_LOG_INFO, DLT_STRING(line));
			sleep(delay);
		}

		if (line) {
			free(line);
		}

		DLT_UNREGISTER_CONTEXT(context);
		DLT_UNREGISTER_APP();
		return 0;
	}
}
