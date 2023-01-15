# Detect OS
ifeq ($(OS), Windows_NT)
	OS := WIN
	LINK := -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

	IPATH := -I.\SDL\include
	LPATH := -L.\SDL\lib

else
	OS := NIX

	LINK := -lSDL2

	IPATH :=
	LPATH :=
endif

DEF := -D$(OS) -DWINDOW_WIDTH=1920/2 -DWINDOW_HEIGHT=1080/2

NAME := game.exe

DEP := jdata.o font.o image.o main.o

all : ico.res project $(DEP)
	@echo =====Empty build folder=====
	@echo . >> ./bin/file.txt
	rm -r ./bin/*
	@echo
	@echo =====Building for $(OS)=====
	gcc -o bin/$(NAME) $(DEP) $(DEF) $(LPATH) $(LINK) ico.res
	@echo
	@echo =====Copy Assets to folder=====
	cp -r Assets bin/
	cp -r SDL2.dll bin/
	cp -r SDL2_ttf.dll bin/
	@echo

jdata.o : jdata.c
	@echo =====Compile jdata.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c jdata.c
	@echo

font.o : font.c
	@echo =====Compile font.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c font.c
	@echo

image.o : image.c
	@echo =====Compile image.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c image.c
	@echo

main.o : main.c
	@echo =====Compie main.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c main.c
	@echo

project :
	@echo =====Create project directory=====
	mkdir ./bin
	@echo Do not delete >> project
	@echo

ico.res : ico.rc
	@echo =====Build icon=====
	windres ico.rc -O coff -o ico.res
	@echo

clean :
	@echo =====Removing all past build files=====
	rm -r ./bin
	rm ./*.o
	rm ico.res
	rm project
