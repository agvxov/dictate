/* @BAKE
    gcc   -o $*-gcc.out   $@ dictate.c -std=c23 -Wall -Wpedantic -ggdb -fsanitize=address
    clang -o $*-clang.out $@ dictate.c -std=c23 -Wall -Wpedantic -ggdb
    ./$*-gcc.out; echo
    ./$*-clang.out
   @STOP
*/
#include "dictate.h"

signed main(void) {
  dictate("$B## Dictate$0\n");
    dictate(10, " / ", 3, " = ", 10.0 / 3.0, "\n");
    dictate("$rthis is a$0 Hungarian $gflag$0\n");
    dictate_color_enabled(0);
    dictate("$rthis$0 is $gnot$0\n");
    dictate_color_enabled(1);

  dictate("$B## mDictate$0\n");
    fmdictate(stderr, "---", "\n\n\n");
    mdictate("$r###$0 ", "this following text\nhas a margin\non the left\n");
    mdictate(" $b.oO$0 ",
        "There are ", 2, " hard problems in programming:\n",
        0, ") naming things\n",
        1, ") cache misses\n",
        2, ") off by one errors\n"
    );

  dictate("$B## Dictatef$0\n");
    dictatef("this is a padded number: %6d\n", 10);
    dictatef("this too is padded here: %*d\n", 6, 10);
    dictatef("my values are 3 6 9 twelve: $b%d %d %d %s$0\n", 3, 6, 9, "twelve");
    dictate("using %s a %x format specifier %d without 'f' does nothing\n");

    return 0;
}
