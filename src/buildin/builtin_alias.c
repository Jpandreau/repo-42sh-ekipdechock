/*
** EPITECH PROJECT, 2026
** repo-42sh-ekipdechock
** File description:
** builtin_alias
*/

#include "my.h"
#include "shell.h"
#include "base.h"

static void display_aliases(char **aliases)
{
    if (aliases == NULL)
        return;
    for (int i = 0; aliases[i] != NULL; i++) {
        my_putstr(aliases[i]);
        my_putchar('\n');
    }
}

static int find_alias_index(char **aliases, char *key)
{
    int key_len = my_strlen(key);
    int i = 0;

    if (aliases == NULL)
        return -1;
    while (aliases[i] != NULL) {
        if (my_strncmp(aliases[i], key, key_len) == 0
            && aliases[i][key_len] == '=')
            return i;
        i++;
    }
    return -1;
}

static char *build_alias_entry(char *key, char *value)
{
    char *temp = my_strcat(key, "=");
    char *entry = NULL;

    if (temp == NULL)
        return NULL;
    entry = my_strcat(temp, value);
    free(temp);
    return entry;
}

static int set_alias(char ***aliases, char *key, char *value)
{
    int i = find_alias_index(*aliases, key);
    char *entry = build_alias_entry(key, value);

    if (entry == NULL)
        return 84;
    if (i != -1) {
        free((*aliases)[i]);
        (*aliases)[i] = entry;
        return 0;
    }
    *aliases = my_array_add(*aliases, entry);
    if (*aliases == NULL)
        return 84;
    return 0;
}

static int unalias(char ***aliases, char *key)
{
    int i = find_alias_index(*aliases, key);

    if (i == -1)
        return 0;
    free((*aliases)[i]);
    while ((*aliases)[i] != NULL) {
        (*aliases)[i] = (*aliases)[i + 1];
        i++;
    }
    return 0;
}

char *get_alias(char **aliases, char *key)
{
    int i = find_alias_index(aliases, key);

    if (i == -1)
        return NULL;
    return aliases[i] + my_strlen(key) + 1;
}

int alias_builtin_args(char **args, char ***aliases)
{
    int size = array_size(args);

    if (size == 1) {
        display_aliases(*aliases);
        return 0;
    }
    if (size == 2) {
        my_putstr(get_alias(*aliases, args[1]));
        my_putchar('\n');
        return 0;
    }
    return set_alias(aliases, args[1], args[2]);
}

int unalias_builtin_args(char **args, char ***aliases)
{
    int size = array_size(args);

    if (size != 2) {
        my_putstr("unalias: usage: unalias name\n");
        return 1;
    }
    return unalias(aliases, args[1]);
}
