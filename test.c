// @BAKE gcc -o $*.out $@ dictate.c -ggdb -Wall -Wpedantic
#include "dictate.h"

signed main(void) {
    dictate("$rthis $gstring $bis $yvery $mcolorful $ckek$0");
    dictate_color_enabled(0);
    dictate("$rthis $gstring $cnot $bis $yvery $mcolorful$0");
    dictate_color_enabled(1);

    dictate("--------");
    dictatef("my values are 3 6 9 twelve: $b%d %d %d %s$0\n", 3, 6, 9, "twelve");

    dictate("--------");
    dictatef("$ysomething uninteresting is here$0\n");
    dictatef("this is a padded number: %6d\n", 10);
    dictatef("this too is padded here: %*d\n", 6, 10);

    dictate("--------");
    mdictate('#', "this following text\nhas a margin\non the left");

    dictate("--------");
    dictate("using %s a %x format specifier %d without 'f' does nothing");

    return 0;
}
