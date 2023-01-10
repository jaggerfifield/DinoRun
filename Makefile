TARGET = main
LINK = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

IPATH = .\include
LPATH = .\lib 

FLAGS = -I$(IPATH) -L$(LPATH) -std=c99 -Wall -O $(LINK)

DEP = font.o image.o

all : $(TARGET).o $(DEP)
	gcc $(TARGET).o $(DEP) $(FLAGS)

$(TARGET).o : $(TARGET).c
	gcc $(TARGET).c $(FLAGS) -c

font.o : font.c font.h
	gcc font.c font.h $(FLAGS) -c

image.o : image.c image.h
	gcc image.c image.h $(FLAGS) -c

clean :
	rm ./*.o
	rm ./*.gch
	rm ./dbg.exe

dbg : $(TARGET).o $(DEP)
	gcc $(TARGET).o $(DEP) $(FLAGS) -ggdb -g -o dbg

mem : all
	drmemory.exe -light -count_leaks a.exe
