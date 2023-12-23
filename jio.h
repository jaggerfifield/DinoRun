#include <stdio.h>

int colorize(void);
void info(char*);
void warn(char*);
void error(char*);
void debug(char*);

char* jread(FILE*, void*, size_t);
void jwrite(FILE*, char*);
FILE* jaccess(char*, char*);
void init_files(void);
