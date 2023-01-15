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

DEF := -DWINDOW_WIDTH=1920/2 -DWINDOW_HEIGHT=1080/2

TST := ./Objects/$(OS)/
DEP := ./Objects/$(OS)/jdata.o ./Objects/$(OS)/font.o ./Objects/$(OS)/image.o ./Objects/$(OS)/main.o

all : project main.o image.o font.o jdata.o
	@echo =====Empty build folder=====
	@echo . >> ./bin/file.txt
	rm -r ./bin/*
	@echo
	@echo =====Building for $(OS)=====
	gcc -o bin/DinoRun $(DEP) $(DEF) -D $(OS) $(LPATH) $(LINK)
	@echo
	@echo =====Copy Assets to folder=====
	cp -r Assets bin/
	cp -r SDL2.dll bin/
	cp -r SDL2_ttf.dll bin/
	@echo

jdata.o : jdata.c
	@echo =====Compile jdata.c=====
	gcc $(IPATH) -D $(OS) $(DEF) -Wall -g -c jdata.c -o ./Objects/$(OS)/jdata.o
	@echo

font.o : font.c
	@echo =====Compile font.c=====
	gcc $(IPATH) -D $(OS) $(DEF) -Wall -g -c font.c -o $(TST)font.o
	@echo

image.o : image.c
	@echo =====Compile image.c=====
	gcc $(IPATH) -D $(OS) $(DEF) -Wall -g -c image.c -o ./Objects/$(OS)/image.o
	@echo

main.o : main.c
	@echo =====Compie main.c=====
	gcc $(IPATH) -D $(OS) $(DEF) -Wall -g -c main.c -o ./Objects/$(OS)/main.o
	@echo

project :
	@echo =====Create project directory=====
	mkdir ./bin
	mkdir ./Objects
	mkdir ./Objects/$(OS)
	@echo Do not delete >> project
	@echo

clean :
	@echo =====Removing all past build files=====
	rm -r ./bin
	rm -r ./Objects
	rm project
