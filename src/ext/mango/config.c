#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ext/mango/string.h>
#include <ext/mango/config.h>

static void lines_free(list_t *lines) {
	for (size_t i = 0; i < lines->size; i++)
		string_free((string_t*)list_get(lines, i));
	list_clear(lines);
	list_free(lines);
}

static char *read(const char *path) {
	FILE *f = fopen(path, "r");
    if (!f) return NULL;

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	rewind(f);

	char *buffer = malloc(size + 1);
	if (!buffer) return NULL;

	fread(buffer, 1, size, f);
	buffer[size] = '\0';

	fclose(f);
	return buffer;
}

static void config_parse(string_t *line, string_t *name, string_t *value) {
	int found_separator = 0;

	for (int i = 0; i < string_length(line); i++) {
		if (!found_separator) {
			if (line->value[i] != '=')
				string_putc(name, line->value[i]);
			else
				found_separator = 1;
		} else
			string_putc(value, line->value[i]);
	}

	if (name) string_trim(name);
	if (value) string_trim(value);
}

int config_has(const char *path, const char *name) {
	char *buffer = read(path);
	list_t *lines = readlines(buffer);
	for (size_t i = 0; i < lines->size; i++) {
		string_t *line = (string_t*) list_get(lines, i);

		string_t *line_name = string_init();
		config_parse(line, line_name, NULL);

		if (!strcmp(line_name->value, name)) {
			string_free(line_name);
			lines_free(lines);
			free(buffer);
			return 1;
		}

		string_free(line_name);
	}

	lines_free(lines);
	free(buffer);
	return 0;
}

char *config_get(const char *path, const char *name) {
	char *buffer = read(path);
	list_t *lines = readlines(buffer);
	for (size_t i = 0; i < lines->size; i++) {
		string_t *line = (string_t*) list_get(lines, i);

		string_t *line_name = string_init();
		string_t *line_value = string_init();
		config_parse(line, line_name, line_value);

		if (!strcmp(line_name->value, name)) {
			char *value = malloc(line_value->size);
			memcpy(value, line_value->value, line_value->size);
			
			string_free(line_name);
			string_free(line_value);
			lines_free(lines);
			free(buffer);
			return value;
		}

		string_free(line_name);
		string_free(line_value);
	}

	lines_free(lines);
	free(buffer);
	return NULL;
}