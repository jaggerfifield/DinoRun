#include <SDL3/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <errno.h>

#ifdef WIN
#include <wchar.h>
#include <windows.h>
#endif

enum {JINFO = SDL_LOG_CATEGORY_CUSTOM, JWARN, JERROR, JDEBUG};

FILE* jaccess(char*, char*);
void jwrite(FILE*, char*);
void jread(FILE*, void*, size_t);
void init_files(void);
int f_count(char*);

void info(char* output, ...){
    va_list args;	
    va_start(args, output);

    SDL_LogMessageV(JINFO, SDL_LOG_PRIORITY_INFO, output, args);

    va_end(args);
}

void warn(char* output, ...){
    va_list args;
    va_start(args, output);
	
    SDL_LogMessageV(JWARN, SDL_LOG_PRIORITY_WARN, output, args);

    va_end(args);
}

void error(char* output, ...){
    va_list args;
    va_start(args, output);

    SDL_LogMessageV(JERROR, SDL_LOG_PRIORITY_ERROR, output, args);

    va_end(args);
}

void debug(char* output, ...){
    va_list args;
    va_start(args, output);

    SDL_LogMessageV(JDEBUG, SDL_LOG_PRIORITY_DEBUG, output, args);

    va_end(args);
}

FILE* jaccess(char* path, char* mode){
	FILE* target = fopen(path, mode);

	if(target == NULL){
		error("Can not open file: %s\n", path);
        perror(NULL);
	    exit(0);
    }

	return target;
}

void jwrite(FILE* file, char* content){
	fwrite(content, sizeof(char), strlen(content), file);
}

void jread(FILE* file, void* dst, size_t size){
	fread(dst, size, sizeof(char), file);
}

int f_count(char* path){
    DIR* dir = opendir(path);
    int count = 0;

    if(dir == NULL){
        error("jio.c : could not read directory: %s", path);
        return -1;
    }

    while(readdir(dir) != NULL)
        count++;

    closedir(dir);

    // Subtract 2 for . and ..
    return count-2;
}

void init_files(void){
	SDL_SetLogPriority(JINFO, SDL_LOG_PRIORITY_INFO);
    SDL_SetLogPriority(JWARN, SDL_LOG_PRIORITY_WARN);
    SDL_SetLogPriority(JERROR, SDL_LOG_PRIORITY_ERROR);
    SDL_SetLogPriority(JDEBUG, SDL_LOG_PRIORITY_DEBUG);

    
    SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_INFO, "\x1b[38;2;35;176;237m[Infos]\x1b[0m : ");
    SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_WARN, "\x1b[38;2;237;149;35m[Warns]\x1b[0m : ");
    SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_ERROR, "\x1b[38;2;237;49;35m[Error]\x1b[0m : ");
    SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_DEBUG, "\x1b[38;2;127;127;127m[Debug]\x1b[0m : ");
    
    info("jio.c : Generate log!");

	fclose(jaccess("score", "a+"));
}

#ifdef WIN
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
#endif
