#ifndef DICTATE_H
#define DICTATE_H
/* # Dictate
 * Dictate is a family of output functions with the designed to be comfortable, not robust.
 * It's intended use-case is within the terminal.
 */

#include <stdarg.h>
#include <stdio.h>

/* ## State
 * Dictate has global state for convinience.
 */
// Do not set by hand, might be subject to change
int color_enabled_global__ = 1;
int pedantic_flushing__    = 1;
/* Flush after every print.
 * Useful for debugging when the process could terminate unexpectedly.
 */
void dictate_pedantic_flush(int b) { pedantic_flushing__    = b; }
/* Enable or disable the processing of color sequences (see below).
 * When colors are disabled, color sequences are not printed at all.
 */
void dictate_color_enabled(int b)  { color_enabled_global__ = b; }

/* ## Grammar
 * Dictate functions follow a similar paradigm as <stdio.h>.
 * 
 *   [a] [va] [TARGET] [m] dictate [f]
 *   
 *   a   -> allocate (must be used along side 'str')
 *   va  -> take a va_list (instead of varargs)
 *   m   -> margin; specifies a prefix inserted before each line
 *   f   -> printf style formatted output; if ommited, a new line will be appended
 *   
 *   TARGET:
 *       NOTE -> stdout
 *       f    -> FILE *
 */
void dictate(const char * str);
void fdictate(FILE * f, const char * str);

void mdictate(char margin, const char * str);
void fmdictate(FILE * f, char margin, const char * str);

void dictatef(const char * fmt, ...);
void vadictatef(const char * fmt, va_list args);
void fdictatef(FILE * f, const char * fmt, ...);
void vafdictatef(FILE * f, const char * fmt, va_list args);

void mdictatef(char margin, const char * fmt, ...);
void vamdictatef(char margin, const char * fmt, va_list args);
void fmdictatef(FILE *f, char margin, const char * fmt, ...);
void vafmdictatef(FILE * f, char margin, const char * fmt, va_list args); // NOTE: core function

/* # Format
 * Dictate supports the most common subset of printf formats.
 * - Width specification (hard coded number and *).
 * - Placeholders:
 *     %d  -> Decimal signed long long
 *     %x  -> Hexadecimal number
 *     %s  -> C string
 *     %c  -> Single character
 * - Colors (ineffective if color is disabled):
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

/* # Imeplementation
 */

// Every other function is a wrapper around this one
void vafmdictatef(FILE * f, char margin, const char * fmt, va_list args) {
    inline
    void print_margin(char margin) {
        const int margin_width = 3;
        if (margin) {
            for (int i = 0; i < margin_width; i++) {
                fputc(margin, f);
            }
            fputc(' ', f);
        }
    }

    print_margin(margin);

    for (const char * s = fmt; *s != '\0'; s++) {
        switch (*s) {
            case '$': { // Color handling
                if (color_enabled_global__) {
                    switch (*(++s)) {
                        case 'r': fprintf(f, "\033[31m"); break;
                        case 'g': fprintf(f, "\033[32m"); break;
                        case 'b': fprintf(f, "\033[34m"); break;
                        case 'y': fprintf(f, "\033[33m"); break;
                        case 'm': fprintf(f, "\033[35m"); break;
                        case 'c': fprintf(f, "\033[36m"); break;
                        case 'B': fprintf(f, "\033[1m"); break;
                        case 'I': fprintf(f, "\033[3m"); break;
                        case '0': fprintf(f, "\033[0m"); break;
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
                        case 'B': ;
                        case 'I': ;
                        case '0': break;
                        default: --s; break;
                    }
                }
            } break;

            case '%': { // fmt specifiers
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
                    case '0' ... '9': { // Static width
                        int width;
                        for (width = 0; *s >= '0' && *s <= '9'; s++) {
                            width = width * 10 + (*s - '0');
                        }

                        --s;

                        goto p;
                    case '*': // Dynamic width
                        width = va_arg(args, long long);
                      p:
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
                    print_margin(margin);
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

void fmdictatef(FILE * f, char margin, const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vafmdictatef(f, margin, fmt, args);
    va_end(args);
}

// Wrapping fmdictatef

void dictate(const char * str) {
    fmdictatef(stdout, '\00', str);
    fputs("\n", stdout);
}

void fdictate(FILE * f, const char * str) {
    fmdictatef(f, '\00', str);
    fputs("\n", stdout);
}

void mdictate(char margin, const char * str) {
    fmdictatef(stdout, margin, str);
    fputs("\n", stdout);
}

void fmdictate(FILE *f, char margin, const char * str) {
    fmdictatef(f, margin, str);
    fputs("\n", stdout);
}

// Wrapping vafmdictatef

void dictatef(const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vafmdictatef(stdout, '\00', fmt, args);
    va_end(args);
}

void vadictatef(const char * fmt, va_list args) {
    vafmdictatef(stdout, '\00', fmt, args);
}

void fdictatef(FILE * f, const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vafmdictatef(f, '\00', fmt, args);
    va_end(args);
}

void vafdictatef(FILE * f, const char * fmt, va_list args) {
    vafmdictatef(f, '\00', fmt, args);
}

void mdictatef(char margin, const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vafmdictatef(stdout, margin, fmt, args);
    va_end(args);
}

void vamdictatef(char margin, const char * fmt, va_list args) {
    vafmdictatef(stdout, margin, fmt, args);
}

// Dictate is in the Public Domain, and if say this is not a legal notice, I will sue you.
#endif
