#include <windows.h>
#include <ext/MinHook.h>
#include <jerky.h>

uintptr_t jerky_addr = 0;
FILE *jerky_log = NULL;
char jerky_path[MAX_PATH] = {0};
func1_t __use_item;

static void jlog(const char *msg, ...) {
    if (!jerky_log) return;

    va_list args;
    va_start(args, msg);
    vfprintf(jerky_log, msg, args);
    va_end(args);

    fflush(jerky_log);
}

static int use_item_callback(int id) {
    #ifdef JERKY_DEBUG
        jlog("Item used: %d\n", id);
    #endif

    if (id == JERKY_ID_ITEM) {
        float *hp = (float*)(jerky_addr + JERKY_OFF_HP);
        float old = *hp;
        *hp = min(old + 2.0, 100.0);

        #ifdef JERKY_DEBUG
            jlog("Healed: %f -> %f\n", old, *hp);
        #endif
    }

    return __use_item(id);
}

DWORD WINAPI Main(LPVOID hModule) {
    jerky_addr = (uintptr_t)GetModuleHandle(NULL);

    GetModuleFileName(hModule, jerky_path, MAX_PATH);
    char *slash = strrchr(jerky_path, '\\');
    if (slash) *slash = '\0';

    char logdir[MAX_PATH + 12];
    sprintf(logdir, "%s\\jerky.log", jerky_path);

    jerky_log = fopen(logdir, "w");
    if (!jerky_log) return 1;

    if (MH_Initialize() != MH_OK) {
        jlog("Failed to initialize MinHook\n");
        return 1;
    }

    jlog("Jerky v%s initialized\n", JERKY_VERSION);
    #ifdef JERKY_DEBUG
        jlog("Addr: 0x%08x\n", jerky_addr);
    #endif

    if (MH_CreateHook((LPVOID)(jerky_addr + 0x96a20), (LPVOID)use_item_callback, (LPVOID*)&__use_item) == MH_OK)
        MH_EnableHook((LPVOID)(jerky_addr + 0x96a20));

    return 0;
}