/*
** EPITECH PROJECT, 2026
** builtin_history
** File description:
** history builtin
*/

#include "base.h"
#include "small_headers.h"

#include <stdio.h>

int history_buildin_args(char **args)
{
    history_t *history = NULL;

    if (args == NULL || args[0] == NULL || array_size(args) != 1)
        return 84;
    history = history_current();
    if (history == NULL || history->entries == NULL)
        return 0;
    for (int i = 0; i < history->size; i++)
        dprintf(1, "%d\t%s\n", i + 1, history->entries[i]);
    return 0;
}
