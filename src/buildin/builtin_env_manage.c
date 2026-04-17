/*
** EPITECH PROJECT, 2026
** builtin_env_manage
** File description:
** setenv and unsetenv builtins
*/

#include "base.h"
#include "buildin.h"

static int change_value(char ***env, int i,
    char *key_with_equal, char *value)
{
    int key_size = my_strlen(key_with_equal);
    char *new_value = my_strcat(key_with_equal, value);

    if (new_value == NULL)
        return 84;
    new_value[key_size + my_strlen(value)] = '\0';
    free((*env)[i]);
    (*env)[i] = new_value;
    free(key_with_equal);
    return 0;
}

static int modification_env(char *key_with_equal,
    char *value, char ***env)
{
    int key_size = 0;

    if (key_with_equal == NULL || value == NULL || env == NULL)
        return 84;
    key_size = my_strlen(key_with_equal);
    for (int i = 0; (*env)[i] != NULL; i++) {
        if (my_strncmp(key_with_equal, (*env)[i], key_size) == 0)
            return change_value(env, i, key_with_equal, value);
    }
    *env = my_array_add(*env, my_strcat(key_with_equal, value));
    free(key_with_equal);
    return *env == NULL ? 84 : 0;
}

static void move_elements(char ***env, int index)
{
    int i = index;

    while ((*env)[i] != NULL) {
        (*env)[i] = (*env)[i + 1];
        i++;
    }
}

static int suppr_env(char *name, char ***env)
{
    int i = 0;
    int size = my_strlen(name);

    while ((*env)[i] != NULL) {
        if (my_strncmp((*env)[i], name, size) == 0 &&
            (*env)[i][size] == '=') {
            free((*env)[i]);
            move_elements(env, i);
            return 0;
        }
        i++;
    }
    return 0;
}

static int setenv_with_equal_arg(char *arg, char ***env)
{
    int equal_pos = -1;
    char *key_with_equal = NULL;
    char *value = NULL;

    if (arg == NULL || env == NULL)
        return 84;
    for (int i = 0; arg[i] != '\0'; i++) {
        if (arg[i] == '=') {
            equal_pos = i;
            break;
        }
    }
    if (equal_pos <= 0)
        return modification_env(my_strcat(arg, "="), "", env);
    key_with_equal = my_strndup(arg, equal_pos + 1);
    if (key_with_equal == NULL)
        return 84;
    value = arg + equal_pos + 1;
    return modification_env(key_with_equal, value, env);
}

int setenv_buildin_args(char **args, char ***env)
{
    char *value = NULL;
    int size = array_size(args);

    if (env == NULL)
        return 84;
    if (size == 1)
        return env_buildin_args(args, *env);
    if (size > 3) {
        my_putstr_err("setenv: Too many arguments.\n");
        return 84;
    }
    if (size == 2)
        return setenv_with_equal_arg(args[1], env);
    value = size == 2 ? "" : args[2];
    return modification_env(my_strcat(args[1], "="), value, env);
}

int unsetenv_buildin(char *arg, char ***env)
{
    char **argument = NULL;
    int size = 0;
    int exit_code = 0;

    if (arg == NULL || env == NULL)
        return 84;
    argument = my_str_to_word_array(arg, ' ');
    if (argument == NULL)
        return 84;
    size = array_size(argument);
    if (size < 1) {
        free_array(argument);
        return 84;
    }
    for (int i = 0; i < size; i++)
        exit_code = suppr_env(argument[i], env);
    free_array(argument);
    return exit_code;
}

int unsetenv_buildin_args(char **args, char ***env)
{
    int size = 0;
    int exit_code = 0;

    if (env == NULL)
        return 84;
    size = array_size(args);
    if (size < 2)
        return 84;
    for (int i = 1; i < size; i++)
        exit_code = suppr_env(args[i], env);
    return exit_code;
}
