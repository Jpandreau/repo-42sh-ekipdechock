/*
** EPITECH PROJECT, 2026
** builtin_history
** File description:
** history builtin
*/

#include "base.h"
#include <stdio.h>
#include <readline/history.h>

int history_buildin_args(char **args)
{
    HIST_ENTRY **list = NULL;

    if (args == NULL || args[0] == NULL || array_size(args) != 1)
        return 84;
    list = history_list();
    if (list == NULL)
        return 0;
    for (int i = 0; list[i] != NULL; i++)
        dprintf(1, "%d\t%s\n", i + history_base, list[i]->line);
    return 0;
}
