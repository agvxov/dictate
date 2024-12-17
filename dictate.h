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
/* Flush after every print.
 * Useful for debugging when the process could terminate unexpectedly.
 */
void dictate_pedantic_flush(int b);
/* Enable or disable the processing of color sequences (see below).
 * When colors are disabled, color sequences are not printed at all.
 */
void dictate_color_enabled(int b);

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

// Dictate is in the Public Domain, and if say this is not a legal notice, I will sue you.
#endif
