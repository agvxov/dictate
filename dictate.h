/* # Dictate
 * Dictate is a family of output functions with the designed to be comfortable, not robust.
 * It's intended use-case is within the terminal.
 */

#define DICTATE_PRINTER_COMMON_ARGS FILE * const f, [[ maybe_unused ]] const char * const m, [[ maybe_unused ]] int h, [[ maybe_unused ]] int n

#ifndef DICTATE_H
#define DICTATE_H

/* Require C23.
 * NOTE: It would be possible to backport to C11.
 */
#if __STDC_VERSION__ < 202000 //202311L /*it appears that its not defined while partially implemented?*/
# error "C23 or newer is required"
#endif

/* ## State
 * Dictate has thread local global state for convenience.
 */
/* Flush after every print.
 * Useful for debugging when the process could terminate unexpectedly.
 */
void dictate_pedantic_flush(bool b);
/* Enable or disable the processing of color sequences (see below).
 * When colors are disabled, color sequences are not printed at all.
 * ### Colors sequences:
 *     $r  -> Red
 *     $g  -> Green
 *     $b  -> Blue
 *     $y  -> Yellow
 *     $m  -> Magenta
 *     $c  -> Cyan
 *     $d  -> black ("dark")
 *     $w  -> white
 *     $B  -> Bold
 *     $I  -> Italic
 *     $0  -> Reset
 */
void dictate_color_enabled(bool b);
/* XXX
 */
//void dictate_quoting_enabled(bool b);

/* ## Dictate functions
 * Dictate functions are generic and variadic.
 * The idea is that you pass in whatever and it just prints it.
 * All primitive types are supported out of the box.
 * Margins and colors are parsed appropriately from C strings.
 *
 * <TARGET> [m] dictate
 *
 *   m   -> margin; specifies a prefix inserted before each line
 *
 *   TARGET:
 *       %empty -> stdout
 *       f      -> FILE *
 *
 * NOTE: what you can see below are pseudo-declarations for macro functions
 */
#if 0
void dictate(...);
void mdictate(const char * margin, ...);
void fdictate(FILE * f, ...);
void fmdictate(FILE * f, const char * margin, ...);
#endif

// ################################
// ### MOVE ALONG; MAGICK AHEAD ###
// ################################
#include <stdarg.h>
#include <stdio.h>

#define DICTATE_NARG(...) DICTATE_PP_NARG_(__VA_ARGS__,DICTATE_PP_RSEQ_N())
#define DICTATE_PP_NARG_(...) DICTATE_PP_128TH_ARG(__VA_ARGS__)
#define DICTATE_PP_128TH_ARG( \
       _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10, \
      _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
      _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
      _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
      _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
      _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
      _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, \
      _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, \
      _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, \
      _91, _92, _93, _94, _95, _96, _97, _98, _99,_100, \
     _101,_102,_103,_104,_105,_106,_107,_108,_109,_110, \
     _111,_112,_113,_114,_115,_116,_117,_118,_119,_120, \
     _121,_122,_123,_124,_125,_126,_127,N,...) N
#define DICTATE_PP_RSEQ_N() \
    127,126,125,124,123,122,121,120,\
    119,118,117,116,115,114,113,112,\
    111,110,109,108,107,106,105,104,\
    103,102,101,100, 99, 98, 97, 96,\
     95, 94, 93, 92, 91, 90, 89, 88,\
     87, 86, 85, 84, 83, 82, 81, 80,\
     79, 78, 77, 76, 75, 74, 73, 72,\
     71, 70, 69, 68, 67, 66, 65, 64,\
     63, 62, 61, 60, 59, 58, 57, 56,\
     55, 54, 53, 52, 51, 50, 49, 48,\
     47, 46, 45, 44, 43, 42, 41, 40,\
     39, 38, 37, 36, 35, 34, 33, 32,\
     31, 30, 29, 28, 27, 26, 25, 24,\
     23, 22, 21, 20, 19, 18, 17, 16,\
     15, 14, 13, 12, 11, 10,  9,  8,\
      7,  6,  5,  4,  3,  2,  1,  0

// trivial type printers
static inline void dictate_bool    (DICTATE_PRINTER_COMMON_ARGS, const bool i)               { fprintf(f, i ? "true" : "false"); }
static inline void dictate_char    (DICTATE_PRINTER_COMMON_ARGS, const char i)               { fprintf(f, "%c", i); }
static inline void dictate_uchar   (DICTATE_PRINTER_COMMON_ARGS, const unsigned char i)      { fprintf(f, "%c", i); }
static inline void dictate_short   (DICTATE_PRINTER_COMMON_ARGS, const short i)              { fprintf(f, "%hd", i); }
static inline void dictate_ushort  (DICTATE_PRINTER_COMMON_ARGS, const unsigned short i)     { fprintf(f, "%hu", i); }
static inline void dictate_int     (DICTATE_PRINTER_COMMON_ARGS, const int i)                { fprintf(f, "%d", i); }
static inline void dictate_uint    (DICTATE_PRINTER_COMMON_ARGS, const unsigned int i)       { fprintf(f, "%u", i); }
static inline void dictate_long    (DICTATE_PRINTER_COMMON_ARGS, const long i)               { fprintf(f, "%ld", i); }
static inline void dictate_ulong   (DICTATE_PRINTER_COMMON_ARGS, const unsigned long i)      { fprintf(f, "%lu", i); }
static inline void dictate_llong   (DICTATE_PRINTER_COMMON_ARGS, const long long i)          { fprintf(f, "%lld", i); }
static inline void dictate_ullong  (DICTATE_PRINTER_COMMON_ARGS, const unsigned long long i) { fprintf(f, "%llu", i); }
static inline void dictate_float   (DICTATE_PRINTER_COMMON_ARGS, const float i)              { fprintf(f, "%f", i); }
static inline void dictate_double  (DICTATE_PRINTER_COMMON_ARGS, const double i)             { fprintf(f, "%lf", i); }
static inline void dictate_ldouble (DICTATE_PRINTER_COMMON_ARGS, const long double i)        { fprintf(f, "%Lf", i); }
static inline void dictate_ptr     (DICTATE_PRINTER_COMMON_ARGS, const void * i)             { fprintf(f, "%p", i); }
// trivial type pointer printers
#define DICTATE_TYPE_POINTER_WRAP(F, t) \
static inline void F ## _ptr (DICTATE_PRINTER_COMMON_ARGS, t * i) { dictate_ptr(f,m,h,n,(void*)i); fputc(' ',f); F(f,m,h,n,*i); }
DICTATE_TYPE_POINTER_WRAP(dictate_bool, bool)
DICTATE_TYPE_POINTER_WRAP(dictate_char, char)
DICTATE_TYPE_POINTER_WRAP(dictate_uchar, unsigned char)
DICTATE_TYPE_POINTER_WRAP(dictate_short, short)
DICTATE_TYPE_POINTER_WRAP(dictate_ushort, unsigned short)
DICTATE_TYPE_POINTER_WRAP(dictate_int, int)
DICTATE_TYPE_POINTER_WRAP(dictate_uint, unsigned int)
DICTATE_TYPE_POINTER_WRAP(dictate_long, long)
DICTATE_TYPE_POINTER_WRAP(dictate_ulong, unsigned long)
DICTATE_TYPE_POINTER_WRAP(dictate_llong, long long)
DICTATE_TYPE_POINTER_WRAP(dictate_ullong, unsigned long long)
DICTATE_TYPE_POINTER_WRAP(dictate_float, float)
DICTATE_TYPE_POINTER_WRAP(dictate_double, double)
DICTATE_TYPE_POINTER_WRAP(dictate_ldouble, long double)

// complex type printers
extern void dictate_str(DICTATE_PRINTER_COMMON_ARGS, const char * const str);

// null object type to fool _Generic into our plot
struct dictatenullt { const void * x; };

/* Here we try our best to optimize away all costs of our macro magick.
 *
 * GCC is always 0 cost like this.
 *
 * Clang also optimizes away the calls, but without -O1 or higher,
 *  it still constructs a swarm of our null objects.
 * An empty struct is non-standard, yet clang can handle it,
 *  however it still allocates 1 byte for them each,
 *  so thats a non-solution.
 */
#ifdef __GNUC__
# ifdef __clang__
#  define _DICATATE_NOOP_ATTRIBUTES __attribute__((always_inline))
# else // gcc
#  define _DICATATE_NOOP_ATTRIBUTES __attribute__((optimize(3)))
# endif
#else
# define _DICATATE_NOOP_ATTRIBUTES
#endif

static inline _DICATATE_NOOP_ATTRIBUTES
void noop(
    [[ maybe_unused ]] const FILE * const f,
    [[ maybe_unused ]] const char * const m,
    [[ maybe_unused ]] int h,
    [[ maybe_unused ]] int n,
    [[ maybe_unused ]] const struct dictatenullt i
) {
    return;
}

#ifndef DICTATE_USER_TYPES
# define DICTATE_USER_TYPES
#endif

#define DICTATE_SWITCH(f, m, h, n, t) _Generic((t) \
        /* special */ \
        , default: noop \
        , void*: dictate_ptr \
        , char*: dictate_str \
        /* primitives */ \
        , bool: dictate_bool \
        , char: dictate_char \
        , unsigned char: dictate_uchar \
        , short: dictate_short \
        , unsigned short: dictate_ushort \
        , int: dictate_int \
        , unsigned int: dictate_uint \
        , long: dictate_long \
        , unsigned long: dictate_ulong \
        , long long: dictate_llong \
        , unsigned long long: dictate_ullong \
        , float: dictate_float \
        , double: dictate_double \
        , long double: dictate_ldouble \
        /* primitive pointers */ \
        , bool*: dictate_bool_ptr \
        , unsigned char*: dictate_uchar_ptr \
        , short*: dictate_short_ptr \
        , unsigned short*: dictate_ushort_ptr \
        , int*: dictate_int_ptr \
        , unsigned int*: dictate_uint_ptr \
        , long*: dictate_long_ptr \
        , unsigned long*: dictate_ulong_ptr \
        , long long*: dictate_llong_ptr \
        , unsigned long long*: dictate_ullong_ptr \
        , float*: dictate_float_ptr \
        , double*: dictate_double_ptr \
        , long double*: dictate_ldouble_ptr \
        /* user types */ \
        DICTATE_USER_TYPES \
    )(f, m, h, n, t)

#define DICTATE_BIG_GUY(\
           n,    f,    m,   _4,   _5,   _6,   _7,   _8,   _9,  _10,  _11,  _12,  _13,  _14,  _15, _16,\
         _17,  _18,  _19,  _20,  _21,  _22,  _23,  _24,  _25,  _26,  _27,  _28,  _29,  _30,  _31, _32,\
         _33,  _34,  _35,  _36,  _37,  _38,  _39,  _40,  _41,  _42,  _43,  _44,  _45,  _46,  _47, _48,\
         _49,  _50,  _51,  _52,  _53,  _54,  _55,  _56,  _57,  _58,  _59,  _60,  _61,  _62,  _63, _64,\
         _65,  _66,  _67,  _68,  _69,  _70,  _71,  _72,  _73,  _74,  _75,  _76,  _77,  _78,  _79, _80,\
         _81,  _82,  _83,  _84,  _85,  _86,  _87,  _88,  _89,  _90,  _91,  _92,  _93,  _94,  _95, _96,\
         _97,  _98,  _99, _100, _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, _111, _112,\
        _113, _114, _115, _116, _117, _118, _119, _120, _121, _122, _123, _124, _125, _126, _127, \
        ...\
    ) \
    DICTATE_SWITCH(f,m,  1,n,_4);   \
    DICTATE_SWITCH(f,m,  2,n,_5);     DICTATE_SWITCH(f,m,  3,n,_6);     DICTATE_SWITCH(f,m,  4,n,_7);     DICTATE_SWITCH(f,m, 5,n,_8);   \
    DICTATE_SWITCH(f,m,  6,n,_9);     DICTATE_SWITCH(f,m,  7,n,_10);    DICTATE_SWITCH(f,m,  8,n,_11);    DICTATE_SWITCH(f,m, 9,n,_12);   \
    DICTATE_SWITCH(f,m, 10,n,_13);    DICTATE_SWITCH(f,m, 11,n,_14);    DICTATE_SWITCH(f,m, 12,n,_15);    DICTATE_SWITCH(f,m, 13,n,_16);   \
    DICTATE_SWITCH(f,m, 14,n,_17);    DICTATE_SWITCH(f,m, 15,n,_18);    DICTATE_SWITCH(f,m, 16,n,_19);    DICTATE_SWITCH(f,m, 17,n,_20);   \
    DICTATE_SWITCH(f,m, 18,n,_21);    DICTATE_SWITCH(f,m, 19,n,_22);    DICTATE_SWITCH(f,m, 20,n,_23);    DICTATE_SWITCH(f,m, 21,n,_24);   \
    DICTATE_SWITCH(f,m, 22,n,_25);    DICTATE_SWITCH(f,m, 23,n,_26);    DICTATE_SWITCH(f,m, 24,n,_27);    DICTATE_SWITCH(f,m, 25,n,_28);   \
    DICTATE_SWITCH(f,m, 26,n,_29);    DICTATE_SWITCH(f,m, 27,n,_30);    DICTATE_SWITCH(f,m, 28,n,_31);    DICTATE_SWITCH(f,m, 29,n,_32);   \
    DICTATE_SWITCH(f,m, 30,n,_33);    DICTATE_SWITCH(f,m, 31,n,_34);    DICTATE_SWITCH(f,m, 32,n,_35);    DICTATE_SWITCH(f,m, 33,n,_36);   \
    DICTATE_SWITCH(f,m, 34,n,_37);    DICTATE_SWITCH(f,m, 35,n,_38);    DICTATE_SWITCH(f,m, 36,n,_39);    DICTATE_SWITCH(f,m, 37,n,_40);   \
    DICTATE_SWITCH(f,m, 38,n,_41);    DICTATE_SWITCH(f,m, 39,n,_42);    DICTATE_SWITCH(f,m, 40,n,_43);    DICTATE_SWITCH(f,m, 41,n,_44);   \
    DICTATE_SWITCH(f,m, 42,n,_45);    DICTATE_SWITCH(f,m, 43,n,_46);    DICTATE_SWITCH(f,m, 44,n,_47);    DICTATE_SWITCH(f,m, 45,n,_48);   \
    DICTATE_SWITCH(f,m, 46,n,_49);    DICTATE_SWITCH(f,m, 47,n,_50);    DICTATE_SWITCH(f,m, 48,n,_51);    DICTATE_SWITCH(f,m, 49,n,_52);   \
    DICTATE_SWITCH(f,m, 50,n,_53);    DICTATE_SWITCH(f,m, 51,n,_54);    DICTATE_SWITCH(f,m, 52,n,_55);    DICTATE_SWITCH(f,m, 53,n,_56);   \
    DICTATE_SWITCH(f,m, 54,n,_57);    DICTATE_SWITCH(f,m, 55,n,_58);    DICTATE_SWITCH(f,m, 56,n,_59);    DICTATE_SWITCH(f,m, 57,n,_60);   \
    DICTATE_SWITCH(f,m, 58,n,_61);    DICTATE_SWITCH(f,m, 59,n,_62);    DICTATE_SWITCH(f,m, 60,n,_63);    DICTATE_SWITCH(f,m, 61,n,_64);   \
    DICTATE_SWITCH(f,m, 62,n,_65);    DICTATE_SWITCH(f,m, 63,n,_66);    DICTATE_SWITCH(f,m, 64,n,_67);    DICTATE_SWITCH(f,m, 65,n,_68);   \
    DICTATE_SWITCH(f,m, 66,n,_69);    DICTATE_SWITCH(f,m, 67,n,_70);    DICTATE_SWITCH(f,m, 68,n,_71);    DICTATE_SWITCH(f,m, 69,n,_72);   \
    DICTATE_SWITCH(f,m, 70,n,_73);    DICTATE_SWITCH(f,m, 71,n,_74);    DICTATE_SWITCH(f,m, 72,n,_75);    DICTATE_SWITCH(f,m, 73,n,_76);   \
    DICTATE_SWITCH(f,m, 74,n,_77);    DICTATE_SWITCH(f,m, 75,n,_78);    DICTATE_SWITCH(f,m, 76,n,_79);    DICTATE_SWITCH(f,m, 77,n,_80);   \
    DICTATE_SWITCH(f,m, 78,n,_81);    DICTATE_SWITCH(f,m, 79,n,_82);    DICTATE_SWITCH(f,m, 80,n,_83);    DICTATE_SWITCH(f,m, 81,n,_84);   \
    DICTATE_SWITCH(f,m, 82,n,_85);    DICTATE_SWITCH(f,m, 83,n,_86);    DICTATE_SWITCH(f,m, 84,n,_87);    DICTATE_SWITCH(f,m, 85,n,_88);   \
    DICTATE_SWITCH(f,m, 86,n,_89);    DICTATE_SWITCH(f,m, 87,n,_90);    DICTATE_SWITCH(f,m, 88,n,_91);    DICTATE_SWITCH(f,m, 89,n,_92);   \
    DICTATE_SWITCH(f,m, 90,n,_93);    DICTATE_SWITCH(f,m, 91,n,_94);    DICTATE_SWITCH(f,m, 92,n,_95);    DICTATE_SWITCH(f,m, 93,n,_96);   \
    DICTATE_SWITCH(f,m, 94,n,_97);    DICTATE_SWITCH(f,m, 95,n,_98);    DICTATE_SWITCH(f,m, 96,n,_99);    DICTATE_SWITCH(f,m, 97,n,_100);   \
    DICTATE_SWITCH(f,m, 98,n,_101);   DICTATE_SWITCH(f,m, 99,n,_102);   DICTATE_SWITCH(f,m,100,n,_103);   DICTATE_SWITCH(f,m,101,n,_104);   \
    DICTATE_SWITCH(f,m,102,n,_105);   DICTATE_SWITCH(f,m,103,n,_106);   DICTATE_SWITCH(f,m,104,n,_107);   DICTATE_SWITCH(f,m,105,n,_108);   \
    DICTATE_SWITCH(f,m,106,n,_109);   DICTATE_SWITCH(f,m,107,n,_110);   DICTATE_SWITCH(f,m,108,n,_111);   DICTATE_SWITCH(f,m,109,n,_112);   \
    DICTATE_SWITCH(f,m,110,n,_113);   DICTATE_SWITCH(f,m,111,n,_114);   DICTATE_SWITCH(f,m,112,n,_115);   DICTATE_SWITCH(f,m,113,n,_116);   \
    DICTATE_SWITCH(f,m,114,n,_117);   DICTATE_SWITCH(f,m,115,n,_118);   DICTATE_SWITCH(f,m,116,n,_119);   DICTATE_SWITCH(f,m,117,n,_120);   \
    DICTATE_SWITCH(f,m,118,n,_121);   DICTATE_SWITCH(f,m,119,n,_122);   DICTATE_SWITCH(f,m,120,n,_123);   DICTATE_SWITCH(f,m,121,n,_124);   \
    DICTATE_SWITCH(f,m,122,n,_125);   DICTATE_SWITCH(f,m,123,n,_126);   DICTATE_SWITCH(f,m,124,n,_127);

#define _nfmdictate(...) do { DICTATE_BIG_GUY(__VA_ARGS__, \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), \
        ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}), ((struct dictatenullt){0}) \
    )} while (0)
#define fmdictate(f, m, ...) do { _nfmdictate(DICTATE_NARG(__VA_ARGS__), f, m, __VA_ARGS__);         } while (0)
#define fdictate(f, ...)     do { _nfmdictate(DICTATE_NARG(__VA_ARGS__), f, NULL, __VA_ARGS__);      } while (0)
#define mdictate(m, ...)     do { _nfmdictate(DICTATE_NARG(__VA_ARGS__), stdout, m, __VA_ARGS__);    } while (0)
#define dictate(...)         do { _nfmdictate(DICTATE_NARG(__VA_ARGS__), stdout, NULL, __VA_ARGS__); } while (0)
// ################################
// ################################
// ################################

/* ## Dictatef functions
 * Dictatef functions follow a similar paradigm as <stdio.h>.
 *
 *   [va] <TARGET> [m] dictatef
 *   
 *   va  -> take a va_list (instead of varargs)
 *   m   -> margin; specifies a prefix inserted before each line
 *
 *   TARGET:
 *       %empty -> stdout
 *       f      -> FILE *
 *
 * ### Format
 * Dictatef supports the most common subset of printf formats.
 * - Width specification (hard coded number or *).
 * - Placeholders:
 *     %d  -> Decimal signed long long
 *     %x  -> Hexadecimal number
 *     %s  -> C string
 *     %c  -> Single character
 */
void dictatef(const char * const fmt, ...);
void vadictatef(const char * const fmt, va_list args);
void fdictatef(FILE * const f, const char * const fmt, ...);
void vafdictatef(FILE * const f, const char * const fmt, va_list args);

void mdictatef(const char * const margin, const char * const fmt, ...);
void vamdictatef(const char * const margin, const char * const fmt, va_list args);
void fmdictatef(FILE * const f, const char * const margin, const char * const fmt, ...);
void vafmdictatef(FILE * f, const char * const margin, const char * const fmt, va_list args); // NOTE: core function

/* ## Dictate debugging functions
 */
#ifdef DEBUG
# ifndef DICTATENDEBUG
#  define debug_dictate          dictate
#  define debug_mdictate        mdictate
#  define debug_fdictate        fdictate
#  define debug_fmdictate      fmdictate
#  define debug_dictatef         dictatef
#  define debug_vadictatef     vadictatef
#  define debug_fdictatef       fdictatef
#  define debug_vafdictatef   vafdictatef
#  define debug_mdictatef       mdictatef
#  define debug_vamdictatef   vamdictatef
#  define debug_fmdictatef     fmdictatef
#  define debug_vafmdictatef vafmdictatef
# endif
#else
#  define debug_dictate(...)
#  define debug_mdictate(...)
#  define debug_fdictate(...)
#  define debug_fmdictate(...)
#  define debug_dictatef(...)
#  define debug_vadictatef(...)
#  define debug_fdictatef(...)
#  define debug_vafdictatef(...)
#  define debug_mdictatef(...)
#  define debug_vamdictatef(...)
#  define debug_fmdictatef(...)
#  define debug_vafmdictatef(...)
#endif

// Dictate is in the Public Domain, and if you say this is not a legal notice, I will sue you.
#endif
