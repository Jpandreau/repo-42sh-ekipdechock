/*
** EPITECH PROJECT, 2026
** builtin_env
** File description:
** env builtin helpers
*/

#include "base.h"

char *my_getenv(char *key, char **env)
{
    int i = 0;
    char *value = NULL;

    while (env[i] != NULL) {
        if (my_strncmp(env[i], key, my_strlen(key)) == 0 &&
            env[i][my_strlen(key)] == '=') {
            value = env[i] + my_strlen(key) + 1;
            return value;
        }
        i++;
    }
    return NULL;
}

int env_buildin_args(char **args, char **env)
{
    if (array_size(args) != 1 || env == NULL)
        return 84;
    for (int i = 0; env[i] != NULL; i++) {
        my_putstr(env[i]);
        my_putchar('\n');
    }
    return 0;
}
