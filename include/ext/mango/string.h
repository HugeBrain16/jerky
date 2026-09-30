#ifndef MANGO_STRING_H
#define MANGO_STRING_H

#include <stddef.h>
#include <ext/mango/list.h>

extern void strltrim(char *str);
extern void strrtrim(char *str);
extern void strtrim(char *str);

typedef struct {
	size_t size;
	char *value;
} string_t;

extern string_t *string_init();
extern string_t *string_from(const char *src);
extern int string_putc(string_t *string, char c);
extern int string_puts(string_t *string, const char *str);
extern int string_length(string_t *string);
extern int string_empty(string_t *string);
extern void string_ltrim(string_t *string);
extern void string_rtrim(string_t *string);
extern void string_trim(string_t *string);
extern void string_free(string_t *string);
extern list_t *readlines(const char *buffer);

#endif