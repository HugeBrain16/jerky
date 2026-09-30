#ifndef SH3_JERKY_H
#define SH3_JERKY_H

#include <minwindef.h>
#include <stdint.h>
#include <stdio.h>

#define JERKY_VERSION "1.0"
#define JERKY_DEBUG

#define JERKY_OFF_HP 0x498660
#define JERKY_OFF_ITEM 0x6D2CAB8
#define JERKY_ID_ITEM 21

typedef int (*func1_t)(int);
typedef uint32_t (*func2_t)(int);
typedef void (*func3_t)(float, int);

extern uintptr_t jerky_addr;
extern FILE *jerky_log;
extern char jerky_path[MAX_PATH];
extern func1_t __use_item;
extern func1_t __use_heal;
extern func2_t __get_attr;
extern func3_t __add_hp;

extern DWORD WINAPI Main(LPVOID hModule);
extern DWORD WINAPI Loop(LPVOID hModule);

#endif