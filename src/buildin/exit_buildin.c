/*
** EPITECH PROJECT, 2026
** exit_buildin
** File description:
** exit_buildin
*/

#include "base.h"
#include "buildin.h"
#include "small_headers.h"

int exit_buildin_args(char **args)
{
    int ok = 0;
    int code = 0;
    int size = array_size(args);

    if (size == 1)
        exit(0);
    if (size > 2)
        return 84;
    code = parse_exit_code_arg(args[1], &ok);
    if (!ok)
        return 84;
    exit(code);
}
