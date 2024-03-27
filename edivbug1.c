#ifdef _MSC_VER
# pragma warning (disable: 4996)
#endif
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>

#ifndef __MINGW64__
# include <errno.h>
#endif

#ifdef _WIN32
# include <io.h>
# include <windows.h>
#else
# include <unistd.h>
#endif

#if (_MSC_VER >= 1300)
# define NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
# define NOINLINE __attribute__((noinline))
#else
# define NOINLINE
#endif

/* EUCLIDEAN DIVISION References
 * [1] https://www.microsoft.com/en-us/research/publication/division-and-modulus-for-computer-scientists/
 * [2] https://dl.acm.org/doi/10.1145/128861.128862
 * [3] https://en.wikipedia.org/wiki/Modulo
 */

/* Euclidean division */
div_t ediv(int a, int b) {
    div_t ret;
    ret.quot = a / b;
    ret.rem  = a % b;
    if (b < 0) {
        ret.quot -= (ret.rem < 0 ? -1 : 0);
        ret.rem  -= (ret.rem < 0 ? ~0 : 0) & b;
    } else {
        ret.quot += (ret.rem < 0 ? -1 : 0);
        ret.rem  += (ret.rem < 0 ? ~0 : 0) & b;
    }
    return ret;
}

/* Euclidean (or floored) division by 4
 * Optimized using SAR
 * For twos-complement signed integer only */
div_t edivby4(int a) {
    div_t ret;
    ret.quot = a >> 2;
    ret.rem = a & 3;
    return ret;
}

/* Right shift for negative signed integer is implementation-defined in C,
 * but MSVC and GCC perform arithmetic right shift for signed integer type
 * and C++20 guarantees this behavior.
 * See [1] https://stackoverflow.com/questions/28681131/right-shift-for-signed-integer
 *     [2] https://learn.microsoft.com/en-us/cpp/cpp/left-shift-and-right-shift-operators-input-and-output?view=msvc-170#right-shifts
 *     [3] https://gcc.gnu.org/onlinedocs/gcc/Integers-implementation.html
 *     [4] https://stackoverflow.com/questions/76495063/how-can-i-reliably-perform-an-arithmetic-shift-right-in-c/76496293#76496293
 *     [5] https://en.wikipedia.org/wiki/Arithmetic_shift#Handling_the_issue_in_programming_languages
 */

/* NOTE:
 * 601295415 == maximum of (int)(a / 25 * (1LL << 35) - a * 1374389535LL)
 *              where `int a` is a multiple of 25
 *              (The maximum occurs at a == -2147483625)
 *           == (int)((1LL << 35) - 25 * 1374389535LL) * (INT_MIN / 25)
 *           == (-7) * (INT_MIN / 25)
 *           == (-7) * (-85899345)
 * 
 * Adding 601295415 before SAR by 37 (or 39) COMPENSATES for the error
 * that occurs when dividend is NEGATIVE and A MULTIPLE OF 100 (or 400)
 */

/* Euclidean (or floored) division by 100
 * Optimized using IMUL and SAR
 * For 32-bit twos-complement signed integer only */
div_t edivby100(int a) {
    div_t ret;
    ret.quot = (int)((a * 1374389535LL + 601295415) >> 37);
    ret.rem = a - 100 * ret.quot;
    return ret;
}

/* Euclidean (or floored) division by 400
 * Optimized using IMUL and SAR
 * For 32-bit twos-complement signed integer only */
div_t edivby400(int a) {
    div_t ret;
    ret.quot = (int)((a * 1374389535LL + 601295415) >> 39);
    ret.rem = a - 400 * ret.quot;
    return ret;
}

/* '--all' option in command line
 *    |
 *    | exist                            +-----------------------------+
 *    +----------> opt_all = 1; -------> | fp = fopen(filename, "wb"); |
 *    |                                  +-----------------------------+
 *    | not exist                             failed  |  
 *    +----------> opt_all = 0;             +---------+
 *                     |                   /          | succeeded
 *                     V                  /           |
 *         +---------------------+       /            V
 *         |     fp = stderr;    | <----+     +----------------+
 *         | prefix = "error: "; |            |  prefix = "";  |
 *         +---------------------+            +----------------+
 *       * Print error msg to stderr        * Print error msg to file
 *       * printerr() returns 1             * printerr() returns 0
 *       * Abort on first error             * Catch all errors
 * 
 * The boxed part is executed when printerr() is called for the first time.
 * If printerr() has never been called before, printerr(..., 0, ..., ..., ...)
 * returns 1 since the default value of prefix is "error: ".
 */ 

int opt_all = 0;

void do_print(int a, int b, div_t ret1, div_t ret2, int ansi, FILE *fp, char *prefix) {
    char *ansi_st = "", *ansi_ed = "";
    if (ansi && a % b == 0 && isatty(fileno(fp))) {
        ansi_st = "\033[32m"; ansi_ed = "\033[0m";
    }
    if (!prefix) prefix = "";
    fprintf(fp, "%s%s%11d %3d | %10d %3d | %10d %3d%s\n", prefix, ansi_st,
            a, b, ret1.quot, ret1.rem, ret2.quot, ret2.rem, ansi_ed);
}

int printerr(int a, int b, div_t ret1, div_t ret2, int ansi) {
    static FILE *fp = NULL;
    static char *prefix = "error: ";
    char filename[32];

    if (b == 0) {
        if (a == 0 && fp && fileno(fp) != 2) {fclose(fp); fp = NULL;}
        return (*prefix ? 1 : 0);
    }

    if (!fp) {
        if (opt_all) {
            char *s = "\033[91m", *e = "\033[0m";
            if (!ansi || !isatty(1)) s = e = "";

            sprintf(filename, "bug-%010lld.txt", (long long)time(NULL));
            fp = fopen(filename, "wb");
            if (fp) {
                printf("%sError messages will be saved to %s\n"
                       "THE FILE WILL TAKE UP 25 GiB (+ 200 B) OF SPACE%s\n",
                       s, filename, e);
                prefix = "";
            } else {
                printf("%sCan't create %s: %s\n"
                       "Error messages will be printed to stderr%s\n",
                       s, filename, strerror(errno), e);
            }
        }
        if (!fp) fp = stderr;
        fprintf(fp, "%s-------------------------------------------------\n"
                    "%s                |     ediv(a, b) |     edivbyb(a)\n"
                    "%s          a   b |       quot rem |       quot rem\n"
                    "%s-------------------------------------------------\n",
                prefix, prefix, prefix, prefix);
    }

    do_print(a, b, ret1, ret2, 0, fp, prefix);
    return (*prefix ? 1 : 0);
}

int NOINLINE test_edivbybs(int ansi) {
    int a, b, r = 0;
    div_t ret1, ret2;
    clock_t st, ed;

    printf("Testing edivby4(), edivby100(), and edivby400()...\n");
    fflush(stdout);

    st = clock();
    for (a = INT_MIN; ; a++) {
        /* Division by 4 */
        ret1 = ediv(a, 4);
        ret2 = edivby4(a);
        if (ret1.quot != ret2.quot || ret1.rem != ret2.rem) {
            r = 1; b = 4;
            if (printerr(a, b, ret1, ret2, ansi)) break;
        }

        /* Division by 100 */
        ret1 = ediv(a, 100);
        ret2 = edivby100(a);
        if (ret1.quot != ret2.quot || ret1.rem != ret2.rem) {
            r = 1; b = 100;
            if (printerr(a, b, ret1, ret2, ansi)) break;
        }

        /* Division by 400 */
        ret1 = ediv(a, 400);
        ret2 = edivby400(a);
        if (ret1.quot != ret2.quot || ret1.rem != ret2.rem) {
            r = 1; b = 400;
            if (printerr(a, b, ret1, ret2, ansi)) break;
        }

        if (a == INT_MAX) break;
    }
    ed = clock();

    if (r == 0) printf("No errors were found\n");

    /* printerr(..., 0, ..., ..., ...) == 0
     * iff  error messages were saved to file      (r == 1, all errors were catched)
     *
     * printerr(..., 0, ..., ..., ...) == 1
     * iff  error messages were printed to stderr  (r == 1, aborted on first error)
     *      OR no errors were found                (r == 0)
     */

    if (printerr(0, 0, ret1, ret2, 0) == 0 || r == 0) {  /* if not aborted */
        printf("Took %.6f sec\n",
               (double)(ed - st) / (double)CLOCKS_PER_SEC);
    } else {  /* if aborted */
        printf("Took %.6f sec (Aborted)\n",
               (double)(ed - st) / (double)CLOCKS_PER_SEC);
        if (opt_all == 0) {
            char *s = "\033[91m", *e = "\033[0m";
            if (!ansi || !isatty(1)) s = e = "";
            printf("%sUse '--all' option to catch all errors%s"
                   " (needs huge disk space)\n", s, e);
        }
    }

    return r;
}

int NOINLINE enable_ansi() {
#ifdef _WIN32
    HANDLE hout;
    DWORD mode;
    int r;
    hout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hout, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    r = SetConsoleMode(hout, mode);
    return (r ? 1 : 0);
#else
    return 1;
#endif
}

int main(int argc, char *argv[]) {
    int a, i;
    int ansi;

    ansi = enable_ansi();

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--all")) {
            opt_all = 1;
        } else {
            fprintf(stderr, "error: unknown option '%s'\n", argv[i]);
            fprintf(stderr, "usage: edivbyb [--all]\n");
            return 2;
        }
    }

    return test_edivbybs(ansi);
}
