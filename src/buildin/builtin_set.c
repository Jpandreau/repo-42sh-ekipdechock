/*
** EPITECH PROJECT, 2026
** repo-42sh-ekipdechock
** File description:
** builtin_set
*/

/*
** EPITECH PROJECT, 2026
** G-PSU-200-MLN-2-1-42sh-1
** File description:
** builtin_set
*/

#include "my.h"
#include "shell.h"
#include "base.h"

static void display_locals(char **locals)
{
    for (int i = 0; locals[i] != NULL; i++) {
        my_putstr(locals[i]);
        my_putchar('\n');
    }
}

static int find_local_index(char **locals, char *key)
{
    int key_len = my_strlen(key);
    int i = 0;

    while (locals[i] != NULL) {
        if (my_strncmp(locals[i], key, key_len) == 0
            && locals[i][key_len] == '=')
            return i;
        i++;
    }
    return -1;
}

static char *build_entry(char *key, char *value)
{
    char *temp = my_strcat(key, "=");
    char *entry = NULL;

    if (temp == NULL)
        return NULL;
    entry = my_strcat(temp, value);
    free(temp);
    return entry;
}

char *get_local(char **locals, char *key)
{
    int i = find_local_index(locals, key);

    if (i == -1)
        return NULL;
    return locals[i] + my_strlen(key) + 1;
}

int set_local(char ***locals, char *key, char *value)
{
    int i = find_local_index(*locals, key);
    char *entry = build_entry(key, value);

    if (entry == NULL)
        return 84;
    if (i != -1) {
        free((*locals)[i]);
        (*locals)[i] = entry;
        return 0;
    }
    *locals = my_array_add(*locals, entry);
    if (*locals == NULL)
        return 84;
    return 0;
}

int unset_local(char ***locals, char *key)
{
    int i = find_local_index(*locals, key);

    if (i == -1)
        return 0;
    free((*locals)[i]);
    while ((*locals)[i] != NULL) {
        (*locals)[i] = (*locals)[i + 1];
        i++;
    }
    return 0;
}

int set_builtin_args(char **args, char ***locals)
{
    int size = array_size(args);

    if (size == 1) {
        display_locals(*locals);
        return 0;
    }
    if (size == 2)
        return set_local(locals, args[1], "");
    if (size == 3)
        return set_local(locals, args[1], args[2]);
    if (size == 4 && my_strcmp(args[2], "=") == 0)
        return set_local(locals, args[1], args[3]);
    return 0;
}

int unset_builtin_args(char **args, char ***locals)
{
    int size = array_size(args);

    if (size < 2) {
        my_putstr_err("unset: Too few arguments.\n");
        return 1;
    }
    for (int i = 1; i < size; i++)
        unset_local(locals, args[i]);
    return 0;
}
