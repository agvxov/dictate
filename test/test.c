/* @BAKE
    DICTATE_C=../source/dictate.c
    DICTATE_O=../source/dictate.o
    # C
    gcc   -o $*-gcc.out      $@ ${DICTATE_C} -std=c23 -Wall -Wpedantic -ggdb -fsanitize=address
    clang -o $*-clang.out    $@ ${DICTATE_C} -std=c23 -Wall -Wpedantic -ggdb
    # C++
    gcc   -o ${DICTATE_O} -c    ${DICTATE_C} -std=c23 -Wall -Wpedantic -ggdb -fsanitize=address
    g++   -o $*-g++.out      $@ ${DICTATE_O}          -Wall -Wpedantic -ggdb -fsanitize=address
    # Run
    ./$*-gcc.out; echo
    ./$*-clang.out; echo
    ./$*-g++.out
   @STOP
*/
#ifdef __cplusplus
#  include "dictate.hpp"
#  include <vector>
#else
#  include "dictate.h"
#endif

signed main(void) {
  dictate("$B## Dictate$0\n");
    dictate(10, " / ", 3, " = ", 10.0 / 3.0, "\n");
    dictate("$rthis is a$0 Hungarian $gtricolor$0\n");
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

  #ifdef __cplusplus
    std::vector<std::string> v = { "aaa", "bbb", "ccc" };

    dictate(v);
  #endif

    return 0;
}
