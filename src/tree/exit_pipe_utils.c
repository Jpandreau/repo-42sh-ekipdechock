/*
** EPITECH PROJECT, 2026
** exit_pipe_utils
** File description:
** exit_pipe_utils
*/

#include "base.h"

static int is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

static int is_number(char *str)
{
    int i = 0;

    if (str == NULL || str[0] == '\0')
        return 0;
    if (str[i] == '+' || str[i] == '-')
        i++;
    if (!is_digit(str[i]))
        return 0;
    for (; str[i]; i++)
        if (!is_digit(str[i]))
            return 0;
    return 1;
}

int parse_exit_code_arg(char *str, int *ok)
{
    int i = 0;
    int nb = 0;
    int signe = 1;

    if (!is_number(str)) {
        *ok = 0;
        return 0;
    }
    if (str[i] == '-')
        signe = -1;
    if (str[i] == '+' || str[i] == '-')
        i++;
    for (; str[i]; i++)
        nb = nb * 10 + (str[i] - '0');
    *ok = 1;
    nb *= signe;
    for (; nb < 0; nb += 256);
    return nb % 256;
}

int exit_code_from_args(char **args, int *valid)
{
    int ok = 0;
    int size = 0;
    int code = 0;

    if (args == NULL) {
        *valid = 0;
        return 0;
    }
    size = array_size(args);
    if (size == 1) {
        *valid = 1;
        return 0;
    }
    if (size > 2) {
        *valid = 0;
        return 0;
    }
    code = parse_exit_code_arg(args[1], &ok);
    *valid = ok;
    return code;
}
