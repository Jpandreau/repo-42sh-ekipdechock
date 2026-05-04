/*
** EPITECH PROJECT, 2026
** repo-42sh-ekipdechock
** File description:
** builtin_set
*/

#include "base.h"
#include "shell.h"
#include "my.h"

static void display_locals(char **locals)
{
    int i = 0;

    if (locals == NULL)
        return;
    while (locals[i] != NULL) {
        my_putstr(locals[i]);
        my_putchar('\n');
        i++;
    }
}

static int find_local_index(char **locals, char *key)
{
    int key_len = my_strlen(key);
    int i = 0;

    if (locals == NULL)
        return -1;
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
    char **new_locals = NULL;

    if (entry == NULL)
        return 84;
    if (i != -1) {
        free((*locals)[i]);
        (*locals)[i] = entry;
        return 0;
    }
    new_locals = my_array_add(*locals, entry);
    if (new_locals == NULL) {
        free(entry);
        return 84;
    }
    *locals = new_locals;
    return 0;
}

int unset_local(char ***locals, char *key)
{
    int i = find_local_index(*locals, key);

    if (i == -1)
        return 0;
    free((*locals)[i]);
    while ((*locals)[i + 1] != NULL) {
        (*locals)[i] = (*locals)[i + 1];
        i++;
    }
    (*locals)[i] = NULL;
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
    return set_local(locals, args[1], args[2]);
}

int unset_builtin_args(char **args, char ***locals)
{
    int size = array_size(args);

    if (size != 2) {
        my_putstr_err("unset: Too few arguments.\n");
        return 1;
    }
    for (int i = 1; i < size; i++)
        unset_local(locals, args[i]);
    return 0;
}
