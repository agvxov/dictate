#ifndef DICTATE_HPP
#define DICTATE_HPP
/* # Dictatepp
 * Implementation of the dictate interface in C++.
 * NOTE: we depend on the C implementation
 */

#define DICTATEPP_PRINTER_COMMON_ARGS FILE * const f, [[ maybe_unused ]] const char * const m, [[ maybe_unused ]] int h, [[ maybe_unused ]] int n

#include <stdio.h>
#include <utility>

extern "C" {
    void dictate_pedantic_flush(bool b);
    void dictate_color_enabled(bool b);

    void dictatef(const char * const fmt, ...);
    void vadictatef(const char * const fmt, va_list args);
    void fdictatef(FILE * const f, const char * const fmt, ...);
    void vafdictatef(FILE * const f, const char * const fmt, va_list args);

    void mdictatef(const char * const margin, const char * const fmt, ...);
    void vamdictatef(const char * const margin, const char * const fmt, va_list args);
    void fmdictatef(FILE * const f, const char * const margin, const char * const fmt, ...);
    void vafmdictatef(FILE * f, const char * const margin, const char * const fmt, va_list args); // NOTE: core function

    void dictate_str(FILE * const f, const char * const m, int h, int n, const char * const str);
}

// trivial type printers
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const bool i)               { fprintf(f, i ? "true" : "false"); }
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const char i)               { fprintf(f, "%c", i); }
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const unsigned char i)      { fprintf(f, "%c", i); }
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const short i)              { fprintf(f, "%hd", i); }
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const unsigned short i)     { fprintf(f, "%hu", i); }
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const int i)                { fprintf(f, "%d", i); }
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const unsigned int i)       { fprintf(f, "%u", i); }
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const long i)               { fprintf(f, "%ld", i); }
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const unsigned long i)      { fprintf(f, "%lu", i); }
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const long long i)          { fprintf(f, "%lld", i); }
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const unsigned long long i) { fprintf(f, "%llu", i); }
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const float i)              { fprintf(f, "%f", i); }
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const double i)             { fprintf(f, "%lf", i); }
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const long double i)        { fprintf(f, "%Lf", i); }
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const void * i)             { fprintf(f, "%p", i); }
// complex type printers
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const char * i) {
    dictate_str(f, m, h, n, i);
}
// C++ STD type printers
#include <string>
static inline void dictate_switch(DICTATEPP_PRINTER_COMMON_ARGS, const std::string & i) {
    dictate_str(f, m, h, n, i.c_str());
}

template <typename... Args>
void fmdictate(FILE * f, const char * const m, Args&&... args) {
    constexpr std::size_t n = sizeof...(args);
    int i = 0;
    (dictate_switch(f, m, ++i, n, std::forward<Args>(args)), ...);
}
#define fdictate(f, ...) fmdictate(f, NULL, __VA_ARGS__)
#define mdictate(m, ...) fmdictate(stdout, m, __VA_ARGS__)
#define dictate(...)     fmdictate(stdout, NULL, __VA_ARGS__)

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
