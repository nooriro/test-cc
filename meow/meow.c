#include <stdio.h>
#include <locale.h>

#if defined(_WIN32)
# include <windows.h>

static void toggle_cp65001(void) {
    static UINT oldicp, oldocp;

    if (!oldicp && !oldocp) {
        oldicp = GetConsoleCP();
        oldocp = GetConsoleOutputCP();
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
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
#if defined(_WIN32)
    toggle_cp65001();
    atexit(toggle_cp65001);
#endif
    setlocale(LC_CTYPE, "");
    printf("%s\n", "🐱");
    printf("%ls\n", L"🐱");
    return 0;
}
