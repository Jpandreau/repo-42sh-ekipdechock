/*
** EPITECH PROJECT, 2026
** history
** File description:
** interactive history wrappers
*/

#include "base.h"
#include "small_headers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void history_clear_entries(history_t *history)
{
    int i = 0;

    if (history == NULL || history->entries == NULL)
        return;
    for (i = 0; i < history->size; i++)
        free(history->entries[i]);
    free(history->entries);
    history->entries = NULL;
    history->size = 0;
    history->capacity = 0;
}

static int history_grow(history_t *history)
{
    char **new_entries = NULL;
    int new_capacity = history->capacity == 0 ? 8 : history->capacity * 2;

    new_entries = realloc(history->entries, sizeof(char *) * new_capacity);
    if (new_entries == NULL)
        return 84;
    history->entries = new_entries;
    history->capacity = new_capacity;
    return 0;
}

int history_init(history_t *history)
{
    if (history == NULL)
        return 84;
    history_clear_entries(history);
    return 0;
}

void history_destroy(history_t *history)
{
    if (history == NULL)
        return;
    history_clear_entries(history);
}

int history_add(history_t *history, char *line)
{
    char *copy = NULL;

    if (history == NULL)
        return 84;
    if (line == NULL || line[0] == '\0')
        return 0;
    if (history->size >= history->capacity && history_grow(history) == 84)
        return 84;
    copy = my_strdup(line);
    if (copy == NULL)
        return 84;
    history->entries[history->size] = copy;
    history->size++;
    return 0;
}

int history_expand_line(history_t *history, char *line, char **expanded)
{
    int idx = 0;

    if (history == NULL || line == NULL || expanded == NULL)
        return 84;
    *expanded = NULL;
    idx = history_skip_spaces(line);
    if (line[idx] != '!') {
        *expanded = my_strdup(line);
        return *expanded == NULL ? 84 : 0;
    }
    *expanded = history_resolve_bang(history, line, idx);
    if (*expanded != NULL)
        return 0;
    if (line[idx + 1] != '\0')
        dprintf(2, "%s: event not found\n", line + idx + 1);
    else
        my_putstr_err("history: event not found\n");
    return 1;
}
