#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN
#include <wchar.h>
#include <windows.h>
#endif

void logger(char*);
FILE* access(char*, char*);
void write(FILE*, char*);
void read(FILE*, void*, size_t);
void init_files(void);

void info(char* output){
	char message[128];
	sprintf(message, "\x1b[38;2;35;176;237m[Infos]\x1b[0m : %s\n", output);
	printf(message);
	sprintf(message, "[Infos] : %s\n", output);
	logger(message);
}

void warn(char* output){
	char message[128];
	sprintf(message, "\x1b[38;2;237;149;35m[Warns]\x1b[0m : %s\n", output);
	printf(message);
	sprintf(message, "[Warns] : %s\n", output);
	logger(message);
}

void error(char* output){
	char message[128];
	sprintf(message, "\x1b[38;2;237;49;35m[Error]\x1b[0m : %s\n", output);
	printf(message);
	sprintf(message, "[Error] : %s\n", output);
	logger(message);
}

void debug(char* output){
	char message[128];
	sprintf(message, "\x1b[38;2;127;127;127m[Debug]\x1b[0m : %s\n", output);
	printf(message);
	sprintf(message, "[Debug] : %s\n", output);
	logger(message);
}

void logger(char* message){
	FILE* logs = access("log.txt", "a+");
	write(logs, message);
	fclose(logs);
}

FILE* access(char* path, char* mode){
	FILE* target = fopen(path, mode);

	if(target == NULL){
		error("");
		printf("\bCan not open file: %s\n", path);
	}

	return target;
}

void write(FILE* file, char* content){
	fwrite(content, sizeof(char), strlen(content), file);
}

void read(FILE* file, void* dst, size_t size){
	fread(dst, size, sizeof(char), file);
}

void init_files(void){
	fclose(access("log.txt", "w+"));
	fclose(access("Assets/score", "a+"));
}

int colorize(){
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return GetLastError();
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return GetLastError();
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return GetLastError();
    }

    return 0;
}
