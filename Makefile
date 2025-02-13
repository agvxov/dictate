CC = gcc
CFLAGS = -std=c23 -Wall -Wpedantic -O2
AR = ar
ARFLAGS = rcs

main:
	${CC} ${CFLAGS} -shared -fPIC dictate.c -o dictate.so
	${CC} ${CFLAGS} -c dictate.c -o libdictate.o
	${AR} rcs libdictate.a dictate.o

clean:
	-rm dictate.o
	-rm libdictate.so
	-rm libdictate.a
