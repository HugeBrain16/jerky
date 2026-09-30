#include <windows.h>
#include <ext/MinHook.h>
#include <jerky.h>

uintptr_t jerky_addr = 0;
FILE *jerky_log = NULL;
char jerky_path[MAX_PATH] = {0};
func1_t __use_item;
func1_t __use_heal;
func2_t __get_attr;
func3_t __add_hp;

static void jlog(const char *msg, ...) {
    if (!jerky_log) return;

    va_list args;
    va_start(args, msg);
    vfprintf(jerky_log, msg, args);
    va_end(args);

    fflush(jerky_log);
}

static uint32_t get_attr_callback(int id) {
    uint32_t attr = __get_attr(id);

    if (id == JERKY_ID_ITEM) {
        attr &= ~0x0001; // clear normal use
        attr |= 0x1000; // set heal use
    }

    return attr;
}

static int use_heal_callback(int id) {
    #ifdef JERKY_DEBUG
        jlog("Item used heal: %d\n", id);
    #endif

    if (id == JERKY_ID_ITEM) {
        __add_hp(2.0, id);
        __use_item(id);
        return 1;
    }

    return __use_heal(id);
}

static int use_item_callback(int id) {
    #ifdef JERKY_DEBUG
        jlog("Item used generic: %d\n", id);
    #endif

    if (id == JERKY_ID_ITEM)
        __add_hp(2.0, id);

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

    if (MH_CreateHook((LPVOID)(jerky_addr + 0x1eb210), (LPVOID)get_attr_callback, (LPVOID*)&__get_attr) == MH_OK)
        MH_EnableHook((LPVOID)(jerky_addr + 0x1eb210));

    if (MH_CreateHook((LPVOID)(jerky_addr + 0x96a20), (LPVOID)use_item_callback, (LPVOID*)&__use_item) == MH_OK)
        MH_EnableHook((LPVOID)(jerky_addr + 0x96a20));

    if (MH_CreateHook((LPVOID)(jerky_addr + 0x97500), (LPVOID)use_heal_callback, (LPVOID*)&__use_heal) == MH_OK)
        MH_EnableHook((LPVOID)(jerky_addr + 0x97500));

    __add_hp = (func3_t)(jerky_addr + 0x6e850);

    return 0;
}