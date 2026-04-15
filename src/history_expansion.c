/*
** EPITECH PROJECT, 2026
** history_expansion
** File description:
** history expansion helpers
*/

#include "base.h"
#include "small_headers.h"

#include <stdio.h>
#include <readline/history.h>

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

char *history_expand_number(char *line, int idx)
{
    long n = 0;
    HIST_ENTRY *entry = NULL;

    if (line[idx] < '0' || line[idx] > '9')
        return NULL;
    while (line[idx] >= '0' && line[idx] <= '9') {
        n = (n * 10) + (line[idx] - '0');
        idx++;
    }
    if (!history_only_spaces_after(line, idx) || n <= 0)
        return NULL;
    entry = history_get((int)n);
    if (entry == NULL || entry->line == NULL)
        return NULL;
    return my_strdup(entry->line);
}

char *history_expand_prefix(char *line, int idx)
{
    int plen = 0;
    HIST_ENTRY *entry = NULL;

    while (line[idx + plen] && line[idx + plen] != ' ' &&
        line[idx + plen] != '\t')
        plen++;
    if (plen == 0 || !history_only_spaces_after(line, idx + plen))
        return NULL;
    for (int i = history_length; i > 0; i--) {
        entry = history_get(i);
        if (entry != NULL && entry->line != NULL &&
            my_strncmp(entry->line, line + idx, plen) == 0)
            return my_strdup(entry->line);
    }
    return NULL;
}

char *history_resolve_bang(char *line, int idx)
{
    HIST_ENTRY *entry = NULL;

    if (line[idx + 1] == '!' && history_only_spaces_after(line, idx + 2)) {
        entry = history_get(history_length);
        if (entry == NULL || entry->line == NULL)
            return NULL;
        return my_strdup(entry->line);
    }
    if (line[idx + 1] >= '0' && line[idx + 1] <= '9')
        return history_expand_number(line, idx + 1);
    return history_expand_prefix(line, idx + 1);
}
