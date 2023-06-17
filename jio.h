int colorize(void);
void info(char*);
void warn(char*);
void error(char*);
void debug(char*);

char* read(FILE*, void*, size_t);
void write(FILE*, char*);
FILE* access(char*, char*);
void init_files(void);
