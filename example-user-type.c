// @BAKE gcc -o $*.out $@ dictate.c -Wall -Wpedantic -std=c23
#include <stdio.h>
#define DICTATE_H
#include "dictate.h"
#undef DICTATE_H

struct byte { unsigned char b; };
struct byte byte(unsigned char c) { return (struct byte) { .b = c, }; }

static inline
void dictate_byte(DICTATE_PRINTER_COMMON_ARGS, struct byte b) {
    fprintf(f, "%02X", b.b);
}

#define DICTATE_USER_TYPES , struct byte : dictate_byte

#include "dictate.h"

signed main(void) {
    dictate(
        byte('\0'), " ",
        byte('a'), " ",
        byte((char)255), "\n"
    );
}
