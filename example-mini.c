// @BAKE gcc -o $*.out $@ dictate.c -std=c23
#include "dictate.h"
signed main(void) {
    dictate(10, " / ", 3, " = ", 10.0 / 3.0, "\n");
}
