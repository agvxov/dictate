#include "dictate.h"
#include <string.h>

/* ## Dictate Imeplementation
 */

// These might be subject to change
static thread_local int color_enabled_global__ = 1;
static thread_local int pedantic_flushing__    = 1;

void dictate_pedantic_flush(bool b) { pedantic_flushing__    = b; }
void dictate_color_enabled(bool b)  { color_enabled_global__ = b; }

#define PRINT_MARGIN do { \
        if (margin) { \
            file_margin_dictate_conditional_format(f, NULL, false, margin); \
        } \
    } while (0)

// dependency for the core function (recursion with indirection)
static
void file_margin_dictate_conditional_format(
    FILE * const f,
    const char * const margin,
    bool do_process_format,
    const char * const str,
    ...
);

// Every format function is ultimetly a wrapper around this one
static
void vararg_file_margin_dictate_conditional_format(
    FILE * const f,
    const char * const margin,
    bool do_process_format,
    const char * const fmt,
    va_list args
  ) {
    for (const char * s = fmt; *s != '\0'; s++) {
        switch (*s) {
            case '$': { // Color handling
                if (color_enabled_global__) {
                    switch (*(++s)) {
                        case 'r': fputs("\033[31m", f); break; // red
                        case 'g': fputs("\033[32m", f); break; // green
                        case 'b': fputs("\033[34m", f); break; // blue
                        case 'y': fputs("\033[33m", f); break; // yellow
                        case 'm': fputs("\033[35m", f); break; // magenta
                        case 'c': fputs("\033[36m", f); break; // cyan
                        case 'd': fputs("\033[30m", f); break; // dark (black)
                        case 'w': fputs("\033[37m", f); break; // white
                        case 'B': fputs("\033[1m",  f); break;
                        case 'I': fputs("\033[3m",  f); break;
                        case '0': fputs("\033[0m",  f); break;
                        default: --s; break; // Invalid color code, backtrack
                    }
                } else {
                    switch (*(++s)) {
                        // Don't echo valid color sequences
                        case 'r': ;
                        case 'g': ;
                        case 'b': ;
                        case 'y': ;
                        case 'm': ;
                        case 'c': ;
                        case 'd': ;
                        case 'w': ;
                        case 'B': ;
                        case 'I': ;
                        case '0': break;
                        default: --s; break;
                    }
                }
            } break;

            case '%': { // fmt specifiers
                if (!do_process_format) {
                    fputc('%', f);
                    break;
                }

                switch (*(++s)) {
                    case 'd': { // Decimal
                        long long val = va_arg(args, long long);
                        fprintf(f, "%lld", val);
                    } break;
                    case 'x': { // Hex
                        unsigned int val = va_arg(args, unsigned int);
                        fprintf(f, "%x", val);
                    } break;
                    case 's': { // String
                        const char * str = va_arg(args, const char *);
                        fprintf(f, "%s", str);
                    } break;
                    case 'c': { // Char
                        char ch = (char)va_arg(args, int);
                        fprintf(f, "%c", ch);
                    } break;
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                    { // Static width
                        int width;
                        for (width = 0; *s >= '0' && *s <= '9'; s++) {
                            width = width * 10 + (*s - '0');
                        }

                        --s;

                        goto width_switch;
                    case '*': // Dynamic width
                        width = va_arg(args, long long);
                      width_switch:
                        switch (*(++s)) {
                            case 'd': {
                                long long x = va_arg(args, long long);
                                fprintf(f, "%*lld", width, x);
                            } break;
                            case 'x': {
                                unsigned int x = va_arg(args, unsigned int);
                                fprintf(f, "%*x", width, x);
                            } break;
                            case 's': {
                                const char * x = va_arg(args, const char *);
                                fprintf(f, "%*s", width, x);
                            } break;
                            case 'c': {
                                char x = (char)va_arg(args, int);
                                fprintf(f, "%*c", width, x);
                            } break;
                            default: --s; break;
                        }
                    } break;
                    default: --s;
                } break;
            } break;

            case '\n': { // Margin handling
                fputc('\n', f);
                if (*(s+1) != '\0') {
                    PRINT_MARGIN;
                }
            } break;

            default: { // Regular characters
                fputc(*s, f);
            } break;
        }
    }

    if (pedantic_flushing__) {
        fflush(f);
    }
}

static
void file_margin_dictate_conditional_format(
    FILE * const f,
    const char * const margin,
    bool do_process_format,
    const char * const str,
    ...
) {
    va_list args;
    va_start(args, str);
    vararg_file_margin_dictate_conditional_format(f, margin, do_process_format, str, args);
    va_end(args);
}

void vafmdictatef(FILE * const f, const char * const margin, const char * const fmt, va_list args) {
    PRINT_MARGIN;
    vararg_file_margin_dictate_conditional_format(f, margin, true, fmt, args);
}

// Wrapping vafmdictatef

void fmdictatef(FILE * const f, const char * const margin, const char * const fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vafmdictatef(f, margin, fmt, args);
    va_end(args);
}

void dictatef(const char * const fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vafmdictatef(stdout, NULL, fmt, args);
    va_end(args);
}

void vadictatef(const char * const fmt, va_list args) {
    vafmdictatef(stdout, NULL, fmt, args);
}

void fdictatef(FILE * const f, const char * const fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vafmdictatef(f, NULL, fmt, args);
    va_end(args);
}

void vafdictatef(FILE * const f, const char * const fmt, va_list args) {
    vafmdictatef(f, NULL, fmt, args);
}

void mdictatef(const char * const margin, const char * const fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vafmdictatef(stdout, margin, fmt, args);
    va_end(args);
}

void vamdictatef(const char * const margin, const char * const fmt, va_list args) {
    vafmdictatef(stdout, margin, fmt, args);
}

// Complex type printers
void dictate_str(FILE * const f, const char * const margin, int h, int n, const char * const str) {
    if (h == 1) {
        PRINT_MARGIN;
    }

    file_margin_dictate_conditional_format(f, margin, false, str);

    if (h != n
    &&  str[strlen(str)-1] == '\n') {
        PRINT_MARGIN;
    }
}

// Dictate is in the Public Domain, and if you say this is not a legal notice, I will sue you.
