#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN
#include <wchar.h>
#include <windows.h>
#endif

void logger(char*);
FILE* jaccess(char*, char*);
void jwrite(FILE*, char*);
void jread(FILE*, void*, size_t);
void init_files(void);

void parse(char* output, va_list args){
    FILE* log = fopen("log.txt", "a");

    for(; *output != '\0'; ++output){
        switch(*output){
            case '%': {
                switch(*(output+1)){
                    case 'd': {
                        int a = va_arg(args, int);
                        printf("%d", a);
                        fprintf(log, "%d", a);
                        ++output;
                        break; 
                    }case 'f': {
                        double a = va_arg(args, double);
                        printf("%f", a);
                        fprintf(log, "%f", a);
                        ++output;
                        break;
                    }
                }
                break;
            }case '\\': {
                switch(*(output+1)){
                    case 'n':{
                        printf("\n");
                        fprintf(log, "\n");
                        break;
                    }
                }
                break;
            }
            default:
                putchar(*output);
                fprintf(log, "%c", *output);
        }
    }
    putchar('\n');
    fprintf(log, "%c", '\n');

    fclose(log);
}

void info(char* output, ...){
    va_list args;	
    va_start(args, output);

    logger("[Infos] : ");

	printf("\x1b[38;2;35;176;237m[Infos]\x1b[0m : ");
    parse(output, args);

    va_end(args);
}

void warn(char* output, ...){
    va_list args;
    va_start(args, output);
	
    logger("[Warns] : ");

	printf("\x1b[38;2;237;149;35m[Warns]\x1b[0m : ");
    parse(output, args);

    va_end(args);
}

void error(char* output, ...){
    va_list args;
    va_start(args, output);

	logger("[Error] : ");
	
    printf("\x1b[38;2;237;49;35m[Error]\x1b[0m : ");
    parse(output, args);

    va_end(args);
}

void debug(char* output, ...){
    va_list args;
    va_start(args, output);
	
    logger("[Debug] : ");

	printf("\x1b[38;2;127;127;127m[Debug]\x1b[0m : ");
    parse(output, args);

    va_end(args);
}

void logger(char* prefix){
	FILE* log = fopen("log.txt", "a");
    fprintf(log, prefix);
	fclose(log);
}

FILE* jaccess(char* path, char* mode){
	FILE* target = fopen(path, mode);

	if(target == NULL){
		error("Can not open file: %s\n", path);
	}

	return target;
}

void jwrite(FILE* file, char* content){
	fwrite(content, sizeof(char), strlen(content), file);
}

void jread(FILE* file, void* dst, size_t size){
	fread(dst, size, sizeof(char), file);
}

void init_files(void){
	fclose(jaccess("log.txt", "w+"));
	info("Generate log!");

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
