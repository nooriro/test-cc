#ifdef _MSC_VER
# pragma warning (disable: 4996)
#endif
#include <stdio.h>
#include <stdlib.h>

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

void NOINLINE print_header() {
    printf("-------------------------------------------------\n"
           "                |     ediv(a, 4) |     ediv(a, b)\n"
           "          a   b |       quot rem |       quot rem\n"
           "-------------------------------------------------\n");
}

void NOINLINE print_footer() {
    printf("-------------------------------------------------\n");
}

void NOINLINE do_print(int a, int b, div_t ret1, div_t ret2, int ansi) {
    char *s[][2] = {{"", ""}, {"", ""}}, *e[] = {"", ""};
    if (ansi && isatty(1)) {
        if (ret1.quot != ret2.quot) {
            s[0][0] = "\033[91m"; s[0][1] = "\033[92m"; e[0] = "\033[0m";
        }
        if (ret1.rem != ret2.rem)   {
            s[1][0] = "\033[91m"; s[1][1] = "\033[92m"; e[1] = "\033[0m";
        }
    }
    printf("%11d %3d | %s%10d%s %s%3d%s | %s%10d%s %s%3d%s\n", a, b,
           s[0][0], ret1.quot, e[0], s[1][0], ret1.rem, e[1],
           s[0][1], ret2.quot, e[0], s[1][1], ret2.rem, e[1]);
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

int main() {
    /* Removing 'volatile' will make ediv(a, b) also broken in MSVC /O2 option */
    volatile int b = 4;
    int a, ansi;

    ansi = enable_ansi();
    print_header();
    for (a = -2147483647 - 1;; a += 4) {
        do_print(a, 4, ediv(a, 4), ediv(a, b), ansi);
        if (a == -2147483640) a = -16;
        if (a == 8) a = 2147483632;
        if (a == 2147483644) break; 
    }
    print_footer();
    return 0;
}
