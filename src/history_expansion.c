/*
** EPITECH PROJECT, 2026
** history_expansion
** File description:
** history expansion helpers
*/

#include "base.h"
#include "small_headers.h"

#include <stdio.h>

static char *history_get_entry(history_t *history, int index)
{
    if (history == NULL || index <= 0 || index > history->size)
        return NULL;
    return history->entries[index - 1];
}

int history_skip_spaces(char *line)
{
    int i = 0;

    while (line[i] && (line[i] == ' ' || line[i] == '\t'))
        i++;
    return i;
}

int history_only_spaces_after(char *line, int pos)
{
    for (; line[pos]; pos++)
        if (line[pos] != ' ' && line[pos] != '\t')
            return 0;
    return 1;
}

char *history_expand_number(char *line, int idx, history_t *history)
{
    long n = 0;

    if (line[idx] < '0' || line[idx] > '9')
        return NULL;
    while (line[idx] >= '0' && line[idx] <= '9') {
        n = (n * 10) + (line[idx] - '0');
        idx++;
    }
    if (!history_only_spaces_after(line, idx) || n <= 0)
        return NULL;
    if (history_get_entry(history, (int)n) == NULL)
        return NULL;
    return my_strdup(history_get_entry(history, (int)n));
}

char *history_expand_prefix(char *line, int idx, history_t *history)
{
    int plen = 0;

    while (line[idx + plen] && line[idx + plen] != ' ' &&
        line[idx + plen] != '\t')
        plen++;
    if (plen == 0 || !history_only_spaces_after(line, idx + plen))
        return NULL;
    for (int i = history == NULL ? 0 : history->size; i > 0; i--) {
        if (history_get_entry(history, i) != NULL &&
            my_strncmp(history_get_entry(history, i), line + idx, plen) == 0)
            return my_strdup(history_get_entry(history, i));
    }
    return NULL;
}

char *history_resolve_bang(history_t *history, char *line, int idx)
{
    int last = history == NULL ? 0 : history->size;

    if (line[idx + 1] == '!' && history_only_spaces_after(line, idx + 2)) {
        if (history_get_entry(history, last) == NULL)
            return NULL;
        return my_strdup(history_get_entry(history, last));
    }
    if (line[idx + 1] >= '0' && line[idx + 1] <= '9')
        return history_expand_number(line, idx + 1, history);
    return history_expand_prefix(line, idx + 1, history);
}
