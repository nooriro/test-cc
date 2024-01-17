#include <stdio.h>
#include <locale.h>
#include <stddef.h>

#if defined(_WIN32)
# include <windows.h>

static void toggle_cp65001(void) {
    static UINT oldicp, oldocp;

    if (!oldicp && !oldocp) {
        oldicp = GetConsoleCP();
        oldocp = GetConsoleOutputCP();
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
        printf("%u %u %u %u %u %u\n", GetACP(), GetOEMCP(),
            oldicp, oldocp, GetConsoleCP(), GetConsoleOutputCP());
    } else {
        fflush(stdout);
        fflush(stderr);
        SetConsoleCP(oldicp);
        SetConsoleOutputCP(oldocp);
        oldocp = oldicp = 0;
    }
}
#endif

int main() {
    const char *mbsv[] = {"성수", "Sŏngsu", "\U0001F431", NULL};
    const wchar_t *wcsv[] = {L"성수", L"Sŏngsu", L"\U0001F431", NULL};
    int i;

#if defined(_WIN32)
    toggle_cp65001();
    atexit(toggle_cp65001);
#endif
    printf("%s\n", setlocale(LC_CTYPE, ""));

    fputs("MultiByte:", stdout);
    for (i = 0; mbsv[i]; i++) {
        printf(" %s", mbsv[i]);
        fflush(stdout);
    }
    putchar('\n');

    fputs("WideChar: ", stdout);
    for (i = 0; wcsv[i]; i++) {
        printf(" %ls", wcsv[i]);
        fflush(stdout);
    }
    putchar('\n');

    return 0;
}
