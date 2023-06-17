#include <stdio.h>

#ifdef WIN
#include <wchar.h>
#include <windows.h>
#endif

void info(char* output){
	char message[128];
	sprintf(message, "\x1b[38;2;35;176;237m[Infos]\x1b[0m : %s\n", output);
	printf(message);
}

void warn(char* output){
	char message[128];
	sprintf(message, "\x1b[38;2;237;149;35m[Warns]\x1b[0m : %s\n", output);
	printf(message);
}

void error(char* output){
	char message[128];
	sprintf(message, "\x1b[38;2;237;49;35m[Error]\x1b[0m : %s\n", output);
	printf(message);
}

void debug(char* output){
	char message[128];
	sprintf(message, "\x1b[38;2;127;127;127m[Debug]\x1b[0m : %s\n", output);
	printf(message);
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
