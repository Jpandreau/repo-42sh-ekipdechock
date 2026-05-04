/*
** EPITECH PROJECT, 2026
** line_edit_history
** File description:
** history storage for line editor
*/

#include <stdlib.h>
#include "base.h"
#include "line_edition.h"

int history_add(history_t *h, char *line)
{
    char **new_entries = NULL;

    if (h == NULL || line == NULL || line[0] == '\0')
        return 0;
    if (h->size >= h->capacity) {
        new_entries = realloc(h->entries, sizeof(char *) * (h->capacity + 16));
        if (new_entries == NULL)
            return 84;
        h->entries = new_entries;
        h->capacity += 16;
    }
    h->entries[h->size] = my_strdup(line);
    if (h->entries[h->size] == NULL)
        return 84;
    h->size++;
    return 0;
}

void history_free(history_t *h)
{
    int i = 0;

    if (h == NULL || h->entries == NULL)
        return;
    while (i < h->size) {
        free(h->entries[i]);
        i++;
    }
    free(h->entries);
    h->entries = NULL;
    h->size = 0;
    h->capacity = 0;
}
