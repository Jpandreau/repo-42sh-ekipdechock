/*
** EPITECH PROJECT, 2026
** shell_manage
** File description:
** shell_t lifecycle management
*/

#include "base.h"
#include "shell.h"

static char **make_empty_array(void)
{
    char **arr = malloc(sizeof(char *));

    if (arr == NULL)
        return NULL;
    arr[0] = NULL;
    return arr;
}

int shell_init(shell_t *shell)
{
    shell->locals = make_empty_array();
    shell->aliases = make_empty_array();
    if (shell->locals == NULL || shell->aliases == NULL) {
        free(shell->locals);
        free(shell->aliases);
        return 84;
    }
    return 0;
}

void shell_destroy(shell_t *shell)
{
    free_array(shell->locals);
    free_array(shell->aliases);
    shell->locals = NULL;
    shell->aliases = NULL;
}
