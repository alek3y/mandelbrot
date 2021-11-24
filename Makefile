CC := cc
FLAGS := -std=c99 -pedantic -Wall -Werror
LIBS := -lm

SRC := main.c
DEST := mandelbrot

all:
	$(CC) $(FLAGS) $(LIBS) $(SRC) -o $(DEST)

clean:
	-rm $(DEST)
