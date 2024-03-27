#include <stdio.h>
#include <stdlib.h>

#if (_MSC_VER >= 1300)
# define NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
# define NOINLINE __attribute__((noinline))
#else
# define NOINLINE
#endif

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

void NOINLINE do_print(const char *prefix, int a, int b, div_t ret) {
    printf("%s %11d %3d | %10d %3d\n", prefix, a, b, ret.quot, ret.rem);
}

void NOINLINE print_expected_result() {
    const char *prefix = "expected: ";
    int a;
    printf("                 arguments     return_value\n");
    printf("                     a   b         quot rem\n");
    a = -2147483647 - 1; do_print(prefix, a, 4, ediv(a, 4));
    a = -2147483644;     do_print(prefix, a, 4, ediv(a, 4));
    a = -2147483640;     do_print(prefix, a, 4, ediv(a, 4));
    a = -12;             do_print(prefix, a, 4, ediv(a, 4));
    a = -8;              do_print(prefix, a, 4, ediv(a, 4));
    a = -4;              do_print(prefix, a, 4, ediv(a, 4));
}

int main() {
    const char *prefix = "actual:   ";
    int a;
    print_expected_result();
    #ifdef __clang__
    # pragma clang loop unroll(disable)
    #endif
    for (a = -2147483647 - 1;; a += 4) {
        do_print(prefix, a, 4, ediv(a, 4));
        if (a == -2147483640) a = -16;
        if (a == -4) break;
    }
    return 0;
}
