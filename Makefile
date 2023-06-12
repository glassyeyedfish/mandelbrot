CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -g
LDFLAGS = -lSDL2 -lm
SRC = src/*.c

mandelbrot: ${SRC}
	${CC} ${CFLAGS} ${SRC} -o $@ ${LDFLAGS}
