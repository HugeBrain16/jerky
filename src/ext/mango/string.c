#include <stdlib.h>
#include <string.h>
#include <ext/mango/string.h>

void strltrim(char *str) {
    size_t start = 0;
    size_t i = 0;

    if (!str) return;

    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n' || str[start] == '\r')
        start++;

    while (str[start])
        str[i++] = str[start++];

    str[i] = '\0';
}

void strrtrim(char *str) {
    size_t end;

    if (!str) return;

    end = 0;
    while (str[end])
        end++;

    while (end > 0 && (str[end - 1] == ' ' || str[end - 1] == '\t' || str[end - 1] == '\n' || str[end - 1] == '\r'))
        end--;

    str[end] = '\0';
}

void strtrim(char *str) {
    if (!str) return;

    strrtrim(str);
    strltrim(str);
}

string_t *string_init() {
    string_t *string = malloc(sizeof(string_t));
    if (!string) return NULL;

    string->size = 1;
    string->value = malloc(1);
    if (!string->value) {
        free(string);
        return NULL;
    }
    string->value[0] = '\0';

    return string;
}

string_t *string_from(const char *src) {
    string_t *string = string_init();
    string_puts(string, src);

    return string;
}

int string_putc(string_t *string, char c) {
    if (!string) return 0;

    char *new = realloc(string->value, string->size + 1);
    if (!new) return 0;
    string->value = new;

    string->value[string->size - 1] = c;
    string->value[string->size] = '\0';
    string->size += 1;

    return 1;
}

int string_puts(string_t *string, const char *str) {
    while (*str != '\0') {
        if (!string_putc(string, *str)) return 0;
        str++;
    }

    return 1;
}

int string_length(string_t *string) {
    return string->size - 1;
}

int string_empty(string_t *string) {
    if (!string) return 1;
    return string_length(string) == 0;
}

void string_free(string_t *string) {
    if (!string) return;
    if (string->value) free(string->value);
    free(string);
}

void string_ltrim(string_t *string) {
    strltrim(string->value);
    string->size = strlen(string->value) + 1;

    char *new = realloc(string->value, string->size);
    if (!new) return;
    string->value = new;
}

void string_rtrim(string_t *string) {
    strrtrim(string->value);
    string->size = strlen(string->value) + 1;

    char *new = realloc(string->value, string->size);
    if (!new) return;
    string->value = new;
}

void string_trim(string_t *string) {
    string_ltrim(string);
    string_rtrim(string);
}

list_t *readlines(const char *buffer) {
    list_t *lines = malloc(sizeof(list_t));
    list_init(lines);

    string_t *line = string_init();
    while (*buffer != '\0') {
        if (*buffer == '\n') {
            list_push(lines, (string_t*)line);
            line = string_init();
        } else
            string_putc(line, *buffer);
        buffer++;
    }

    if (string_length(line))
        list_push(lines, (string_t*)line);
    else
        string_free(line);

    return lines;
}