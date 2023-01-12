# Detect OS
ifeq ($(OS), Windows_NT)
	OS := WIN
	LINK := -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

	IPATH := -I.\SDL\include
	LPATH := -L.\SDL\lib

	DEP := font.o image.o
else
	OS := NIX

	LINK := -lSDL2

	IPATH :=
	LPATH :=
endif

DEP := ./Objects/$(OS)/image.o ./Objects/$(OS)/main.o

all : project main.o image.o

	@echo =====Empty build folder=====
	@echo . >> ./bin/file.txt
	rm -r ./bin/*
	@echo

	@echo =====Building for $(OS)=====
	gcc -o bin/DinoRun $(DEP) -D $(OS) $(LPATH) $(LINK)
	@echo

	@echo =====Copy Assets to folder=====
	cp -r Assets bin/
	cp -r SDL2.dll bin/
	@echo

image.o : image.c
	@echo =====Compile image.c=====
	gcc $(IPATH) -D $(OS) -Wall -g -c image.c -o ./Objects/$(OS)/image.o
	@echo

main.o : main.c
	@echo =====Compie main.c=====
	gcc $(IPATH) -D $(OS) -Wall -g -c main.c -o ./Objects/$(OS)/main.o
	@echo

project :
	@echo =====Create project directory=====
	mkdir ./bin
	mkdir ./Objects
	mkdir ./Objects/$(OS)
	@echo done >> project
	@echo

clean :
	@echo =====Removing all past build files=====
	rm -r ./bin
	rm -r ./Objects
	rm project
