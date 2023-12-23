# Detect OS
ifeq ($(OS), Windows_NT)
	OS := WIN
	LINK := -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
	RESO := ico.res

	IPATH := -I.\SDL\include
	LPATH := -L.\SDL\lib

	COPY1 := cp -r SDL/SDL2.dll bin/
	COPY2 := cp -r SDL/SDL2_ttf.dll bin/

else
	OS := NIX

	LINK := -lSDL2 -lSDL2_ttf
	RESO :=

	IPATH :=
	LPATH :=
endif

DEF := -D$(OS)

NAME := game

DEP := jtime.o play.o story.o gameover.o menu.o jdata.o jio.o update.o data.o main.o

all : $(RESO) project $(DEP)
	@echo =====Empty build folder=====
	@echo . >> ./bin/file.txt
	rm -r ./bin/*
	@echo
	@echo =====Building for $(OS)=====
	gcc -ggdb -o bin/$(NAME) $(DEP) $(DEF) $(LPATH) $(LINK) $(RESO)
	@echo
	@echo =====Copy Assets to folder=====
	cp -r Assets bin/
	$(COPY1)
	$(COPY2)
	@echo

jtime.o : jtime.c
	@echo =====Compile jtime.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c jtime.c
	@echo

play.o : play.c
	@echo =====Compile play.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c play.c
	@echo

story.o : story.c
	@echo =====Compile story.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c story.c
	@echo

gameover.o : gameover.c
	@echo =====Compile gameover.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c gameover.c
	@echo

menu.o : menu.c
	@echo =====Compile menu.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c menu.c
	@echo

jdata.o : jdata.c
	@echo =====Compile jdata.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c jdata.c
	@echo

jio.o : jio.c
	@echo =====Compile jio.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c jio.c
	@echo

update.o : update.c
	@echo =====Compile update.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c update.c
	@echo

data.o : data.c
	@echo =====Compie data.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c data.c

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
	rm project
