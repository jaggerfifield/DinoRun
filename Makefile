# Detect OS
ifeq ($(OS), Windows_NT)
	OS := WIN
	LINK := -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

	IPATH := .\include
	LPATH := .\lib 

	FLAGS := -I$(IPATH) -L$(LPATH) -std=c99 -Wall -O $(LINK)
	DEP := font.o image.o
else
	OS := NIX

	LINK := -lSDL2

	IPATH := /usr/include/SDL2
	LPATH := /usr/lib
endif

DEP := image.o

all : main.o $(DEP)
	@echo Building for $(OS)
	gcc -o bin/DinoRun $(DEP) main.o -L$(LPATH) $(LINK)
	
	@echo Copy Assets to folder
	cp -r Assets bin/
	cp -r SDL2.dll bin/

image.o : image.c
	gcc -I$(IPATH) -D_REENTRANT -Wall -g -c image.c -o image.o

main.o : main.c
	gcc -I$(IPATH) -D_REENTRANT -Wall -g -c main.c -o main.o

