#ifndef DICTATE_H
#define DICTATE_H
/* # Dictate
 * Dictate is a family of output functions with the designed to be comfortable, not robust.
 * It's intended use-case is within the terminal.
 */

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
void mdictate(char margin, ...);
void fdictate(FILE * f, ...);
void fmdictate(FILE * f, char margin, ...);
#endif

// ################################
// ### MOVE ALONG; MAGICK AHEAD ###
// ################################
#include <stdarg.h>
#include <stdio.h>

// trivial type printers
static inline void dictate_bool    (FILE * const f, [[ maybe_unused ]] const char * const m, const bool i)               { fprintf(f, i ? "true" : "false"); }
static inline void dictate_char    (FILE * const f, [[ maybe_unused ]] const char * const m, const char i)               { fprintf(f, "%c", i); }
static inline void dictate_uchar   (FILE * const f, [[ maybe_unused ]] const char * const m, const unsigned char i)      { fprintf(f, "%c", i); }
static inline void dictate_short   (FILE * const f, [[ maybe_unused ]] const char * const m, const short i)              { fprintf(f, "%hd", i); }
static inline void dictate_ushort  (FILE * const f, [[ maybe_unused ]] const char * const m, const unsigned short i)     { fprintf(f, "%hu", i); }
static inline void dictate_int     (FILE * const f, [[ maybe_unused ]] const char * const m, const int i)                { fprintf(f, "%d", i); }
static inline void dictate_uint    (FILE * const f, [[ maybe_unused ]] const char * const m, const unsigned int i)       { fprintf(f, "%u", i); }
static inline void dictate_long    (FILE * const f, [[ maybe_unused ]] const char * const m, const long i)               { fprintf(f, "%ld", i); }
static inline void dictate_ulong   (FILE * const f, [[ maybe_unused ]] const char * const m, const unsigned long i)      { fprintf(f, "%lu", i); }
static inline void dictate_llong   (FILE * const f, [[ maybe_unused ]] const char * const m, const long long i)          { fprintf(f, "%lld", i); }
static inline void dictate_ullong  (FILE * const f, [[ maybe_unused ]] const char * const m, const unsigned long long i) { fprintf(f, "%llu", i); }
static inline void dictate_float   (FILE * const f, [[ maybe_unused ]] const char * const m, const float i)              { fprintf(f, "%f", i); }
static inline void dictate_double  (FILE * const f, [[ maybe_unused ]] const char * const m, const double i)             { fprintf(f, "%lf", i); }
static inline void dictate_ldouble (FILE * const f, [[ maybe_unused ]] const char * const m, const long double i)        { fprintf(f, "%Lf", i); }
static inline void dictate_ptr     (FILE * const f, [[ maybe_unused ]] const char * const m, const void * i)             { fprintf(f, "%p", i); }
// trivial type pointer printers
#define DICTATE_TYPE_POINTER_WRAP(F, t) \
static inline void F ## _ptr (FILE * const f, [[ maybe_unused ]] const char * const m, t * i) { dictate_ptr(f,m,(void*)i); fputc(' ',f); F(f,m,*i); }
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
extern void dictate_str(FILE * const f, const char * const m, const char * const str);

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
    [[ maybe_unused ]] const struct dictatenullt i
) {
    return;
}

#define DICTATE_SWITCH(f, m, t) _Generic((t) \
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
    )(f, m, t)

#define DICTATE_BIG_GUY(\
           f,    m,   _3,   _4,   _5,   _6,   _7,   _8,   _9,  _10,  _11,  _12,  _13,  _14,  _15, _16,\
         _17,  _18,  _19,  _20,  _21,  _22,  _23,  _24,  _25,  _26,  _27,  _28,  _29,  _30,  _31, _32,\
         _33,  _34,  _35,  _36,  _37,  _38,  _39,  _40,  _41,  _42,  _43,  _44,  _45,  _46,  _47, _48,\
         _49,  _50,  _51,  _52,  _53,  _54,  _55,  _56,  _57,  _58,  _59,  _60,  _61,  _62,  _63, _64,\
         _65,  _66,  _67,  _68,  _69,  _70,  _71,  _72,  _73,  _74,  _75,  _76,  _77,  _78,  _79, _80,\
         _81,  _82,  _83,  _84,  _85,  _86,  _87,  _88,  _89,  _90,  _91,  _92,  _93,  _94,  _95, _96,\
         _97,  _98,  _99, _100, _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, _111, _112,\
        _113, _114, _115, _116, _117, _118, _119, _120, _121, _122, _123, _124, _125, _126, _127, \
        ...\
    ) \
        DICTATE_SWITCH(f,m,_3);   DICTATE_SWITCH(f,m,_4); \
        DICTATE_SWITCH(f,m,_5);   DICTATE_SWITCH(f,m,_6);   DICTATE_SWITCH(f,m,_7);  DICTATE_SWITCH(f,m,_8); \
        DICTATE_SWITCH(f,m,_9);   DICTATE_SWITCH(f,m,_10);  DICTATE_SWITCH(f,m,_11); DICTATE_SWITCH(f,m,_12); \
        DICTATE_SWITCH(f,m,_13);  DICTATE_SWITCH(f,m,_14);  DICTATE_SWITCH(f,m,_15); DICTATE_SWITCH(f,m,_16); \
        DICTATE_SWITCH(f,m,_17);  DICTATE_SWITCH(f,m,_18);  DICTATE_SWITCH(f,m,_19); DICTATE_SWITCH(f,m,_20); \
        DICTATE_SWITCH(f,m,_21);  DICTATE_SWITCH(f,m,_22);  DICTATE_SWITCH(f,m,_23); DICTATE_SWITCH(f,m,_24); \
        DICTATE_SWITCH(f,m,_25);  DICTATE_SWITCH(f,m,_26);  DICTATE_SWITCH(f,m,_27); DICTATE_SWITCH(f,m,_28); \
        DICTATE_SWITCH(f,m,_29);  DICTATE_SWITCH(f,m,_30);  DICTATE_SWITCH(f,m,_31); DICTATE_SWITCH(f,m,_32); \
        DICTATE_SWITCH(f,m,_33);  DICTATE_SWITCH(f,m,_34);  DICTATE_SWITCH(f,m,_35); DICTATE_SWITCH(f,m,_36); \
        DICTATE_SWITCH(f,m,_37);  DICTATE_SWITCH(f,m,_38);  DICTATE_SWITCH(f,m,_39); DICTATE_SWITCH(f,m,_40); \
        DICTATE_SWITCH(f,m,_41);  DICTATE_SWITCH(f,m,_42);  DICTATE_SWITCH(f,m,_43); DICTATE_SWITCH(f,m,_44); \
        DICTATE_SWITCH(f,m,_45);  DICTATE_SWITCH(f,m,_46);  DICTATE_SWITCH(f,m,_47); DICTATE_SWITCH(f,m,_48); \
        DICTATE_SWITCH(f,m,_49);  DICTATE_SWITCH(f,m,_50);  DICTATE_SWITCH(f,m,_51); DICTATE_SWITCH(f,m,_52); \
        DICTATE_SWITCH(f,m,_53);  DICTATE_SWITCH(f,m,_54);  DICTATE_SWITCH(f,m,_55); DICTATE_SWITCH(f,m,_56); \
        DICTATE_SWITCH(f,m,_57);  DICTATE_SWITCH(f,m,_58);  DICTATE_SWITCH(f,m,_59); DICTATE_SWITCH(f,m,_60); \
        DICTATE_SWITCH(f,m,_61);  DICTATE_SWITCH(f,m,_62);  DICTATE_SWITCH(f,m,_63); DICTATE_SWITCH(f,m,_64); \
        DICTATE_SWITCH(f,m,_65);  DICTATE_SWITCH(f,m,_66);  DICTATE_SWITCH(f,m,_67); DICTATE_SWITCH(f,m,_68); \
        DICTATE_SWITCH(f,m,_69);  DICTATE_SWITCH(f,m,_70);  DICTATE_SWITCH(f,m,_71); DICTATE_SWITCH(f,m,_72); \
        DICTATE_SWITCH(f,m,_73);  DICTATE_SWITCH(f,m,_74);  DICTATE_SWITCH(f,m,_75); DICTATE_SWITCH(f,m,_76); \
        DICTATE_SWITCH(f,m,_77);  DICTATE_SWITCH(f,m,_78);  DICTATE_SWITCH(f,m,_79); DICTATE_SWITCH(f,m,_80); \
        DICTATE_SWITCH(f,m,_81);  DICTATE_SWITCH(f,m,_82);  DICTATE_SWITCH(f,m,_83); DICTATE_SWITCH(f,m,_84); \
        DICTATE_SWITCH(f,m,_85);  DICTATE_SWITCH(f,m,_86);  DICTATE_SWITCH(f,m,_87); DICTATE_SWITCH(f,m,_88); \
        DICTATE_SWITCH(f,m,_89);  DICTATE_SWITCH(f,m,_90);  DICTATE_SWITCH(f,m,_91); DICTATE_SWITCH(f,m,_92); \
        DICTATE_SWITCH(f,m,_93);  DICTATE_SWITCH(f,m,_94);  DICTATE_SWITCH(f,m,_95); DICTATE_SWITCH(f,m,_96); \
        DICTATE_SWITCH(f,m,_97);  DICTATE_SWITCH(f,m,_98);  DICTATE_SWITCH(f,m,_99); DICTATE_SWITCH(f,m,_100); \
        DICTATE_SWITCH(f,m,_101); DICTATE_SWITCH(f,m,_102); DICTATE_SWITCH(f,m,_103); DICTATE_SWITCH(f,m,_104); \
        DICTATE_SWITCH(f,m,_105); DICTATE_SWITCH(f,m,_106); DICTATE_SWITCH(f,m,_107); DICTATE_SWITCH(f,m,_108); \
        DICTATE_SWITCH(f,m,_109); DICTATE_SWITCH(f,m,_110); DICTATE_SWITCH(f,m,_111); DICTATE_SWITCH(f,m,_112); \
        DICTATE_SWITCH(f,m,_113); DICTATE_SWITCH(f,m,_114); DICTATE_SWITCH(f,m,_115); DICTATE_SWITCH(f,m,_116); \
        DICTATE_SWITCH(f,m,_117); DICTATE_SWITCH(f,m,_118); DICTATE_SWITCH(f,m,_119); DICTATE_SWITCH(f,m,_120); \
        DICTATE_SWITCH(f,m,_121); DICTATE_SWITCH(f,m,_122); DICTATE_SWITCH(f,m,_123); DICTATE_SWITCH(f,m,_124); \
        DICTATE_SWITCH(f,m,_125); DICTATE_SWITCH(f,m,_126); DICTATE_SWITCH(f,m,_127);

#define fmdictate(...) do { DICTATE_BIG_GUY(__VA_ARGS__, \
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
#define fdictate(f, ...) do { fmdictate(f, NULL, __VA_ARGS__);      } while (0)
#define mdictate(m, ...) do { fmdictate(stdout, m, __VA_ARGS__);     } while (0)
#define dictate(...)     do { fmdictate(stdout, NULL, __VA_ARGS__); } while (0)
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
