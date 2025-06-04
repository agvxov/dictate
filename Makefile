.PHONY: test

CFLAGS := -std=c23 -Wall -Wextra -Wpedantic -O2

main:
	${CC} ${CFLAGS} -shared -fPIC source/dictate.c -o object/libdictate.so
	${CC} ${CFLAGS} -c source/dictate.c -o object/dictate.o
	${AR} rcs object/libdictate.a object/dictate.o

test:
	bake test/test.c

man:
	ts-md2man README.md > object/dictate.3

clean:
	-${RM} object/*{.o,.so,.a}
	-${RM} test/*.out
