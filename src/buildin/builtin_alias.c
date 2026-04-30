/*
** EPITECH PROJECT, 2026
** repo-42sh-ekipdechock
** File description:
** builtin_alias
*/

#include "base.h"
#include "shell.h"
#include "my.h"

static void display_alias(char **aliases)
{
    int i = 0;

    if (aliases == NULL)
        return;
    while (aliases[i] != NULL) {
        my_putstr(aliases[i]);
        my_putchar('\n');
        i++;
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

static int unalias_one(char ***aliases, char *key)
{
    int i = find_alias_index(*aliases, key);

    if (i == -1)
        return 0;
    free((*aliases)[i]);
    while ((*aliases)[i + 1] != NULL) {
        (*aliases)[i] = (*aliases)[i + 1];
        i++;
    }
    (*aliases)[i] = NULL;
    return 0;
}

char *get_alias(char **aliases, char *key)
{
    int i = find_alias_index(aliases, key);

    if (i == -1)
        return NULL;
    return aliases[i] + my_strlen(key) + 1;
}

int set_alias_entry(char ***aliases, char *key, char *value)
{
    char *entry = build_alias_entry(key, value);
    int i = find_alias_index(*aliases, key);

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

static int alias_set_from_token(char ***aliases, char *token)
{
    int eq = 0;
    char *key = NULL;
    int ret = 0;

    while (token[eq] != '\0' && token[eq] != '=')
        eq++;
    if (token[eq] != '=')
        return -1;
    key = my_strndup(token, eq);
    if (key == NULL)
        return 84;
    ret = set_alias_entry(aliases, key, token + eq + 1);
    free(key);
    return ret;
}

int alias_builtin_args(char **args, char ***aliases)
{
    int size = array_size(args);
    char *value = NULL;
    int set_ret = 0;

    if (size == 1) {
        display_alias(*aliases);
        return 0;
    }
    if (size == 2) {
        set_ret = alias_set_from_token(aliases, args[1]);
        if (set_ret != -1)
            return set_ret;
        value = get_alias(*aliases, args[1]);
        if (value != NULL) {
            my_putstr(value);
            my_putchar('\n');
        }
        return 0;
    }
    return set_alias_entry(aliases, args[1], args[2]);
}

int unalias_builtin_args(char **args, char ***aliases)
{
    int size = array_size(args);
    int i = 1;

    if (size < 2) {
        my_putstr_err("Unalias: Too few arguments\n");
        return 1;
    }
    while (i < size) {
        unalias_one(aliases, args[i]);
        i++;
    }
    return 0;
}
