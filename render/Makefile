CC = gcc
CFLAGS = -std=c99 -O0 -g -Wall -Wextra -Wpedantic `sdl2-config --cflags`
INCLUDES = -I.. -Istb
LIBS = -lm

all: render

render: main.o
	$(CC) -o $@ $+ $(LIBS)

main.o: main.c ../al_bdf.h stb/stb_image_write.h
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

clean: FORCE
	rm -f render main.o

.PHONY: FORCE
