# Detect OS
ifeq ($(OS), Windows_NT)
	OS := WIN
	OTHER := NIX
	LINK := -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
	RESO := ico.res

	IPATH := -I.\SDL\include
	LPATH := -L.\SDL\lib

	COPY1 := cp -r SDL/SDL2.dll bin/$(OS)
	COPY2 := cp -r SDL/SDL2_ttf.dll bin/$(OS)
	
	BUILD_DIR := \\

else
	OS := NIX
	OTHER := WIN

	LINK := -lSDL2 -lSDL2_ttf
	RESO :=

	IPATH :=
	LPATH :=

	BUILD_DIR := //
endif

DEF := -D$(OS)

WDIR := $(shell pwd)

NAME := game

DEP := jtime.o play.o story.o gameover.o menu.o settings.o jdata.o jio.o update.o data.o main.o

all : $(RESO) $(OS) $(DEP)
	@echo =====Working Directory=====
	@echo $(WDIR)
	@echo
	@echo =====Empty build folder=====
	@echo . >> ./bin/$(OS)/file.txt
	rm -r ./bin/$(OS)/*
	@echo
	@echo =====Building for $(OS)=====
	gcc -ggdb -o $(WDIR)/bin/$(OS)/$(NAME) $(DEP) $(DEF) $(LPATH) $(LINK) $(RESO)
	@echo
	@echo =====Copy Assets to folder=====
	cp -r Assets bin/$(OS)/
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

settings.o : settings.c
	@echo =====Compile settings.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c settings.c
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
	@echo

main.o : main.c
	@echo =====Compie main.c=====
	gcc $(IPATH) $(DEF) -Wall -g -c main.c
	@echo

$(OS) : $(OS).proj
	@echo =====Updating objects=====
	touch tmp.o
	@echo tmp >> $(OTHER)
	rm $(OTHER)
	rm ./*.o
	@echo Do not delete >> $(OS)
	@echo

$(OS).proj :
	@echo =====Create project directory=====
	mkdir bin$(BUILD_DIR)$(OS)
	@echo Do no delete >> $(OS).proj
	@echo

ico.res : ico.rc
	@echo =====Build icon=====
	windres ico.rc -O coff -o ico.res
	@echo

clean : $(OS)
	@echo =====Removing objects=====
	@echo tmp >> tmp.o
	rm ./*.o
	rm -r ./bin/$(OS)
	rm $(OS)
	rm $(OS).proj
