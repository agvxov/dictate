.PHONY: test

CFLAGS := -std=c23 -Wall -Wpedantic -O2

main:
	${CC} ${CFLAGS} -shared -fPIC source/dictate.c -o object/libdictate.so
	${CC} ${CFLAGS} -c source/dictate.c -o object/dictate.o
	${AR} rcs object/libdictate.a object/dictate.o

test:
	bake test/test.c

clean:
	-${RM} object/*{.o,.so,.a}
	-${RM} test/*.out
