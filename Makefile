CC = gcc
CFLAGS = -std=c23 -Wall -Wpedantic -O2
AR = ar
ARFLAGS = rcs

main:
	${CC} ${CFLAGS} -shared -fPIC dictate.c -o dictate.so
	${CC} ${CFLAGS} -c dictate.c -o dictate.o
	${AR} rcs dictate.a dictate.o

clean:
	-rm dictate.so
	-rm dictate.o
	-rm dictate.a
