/*
** EPITECH PROJECT, 2026
** exit_status
** File description:
** exit_status
*/

#include "tree.h"

int make_exit_status(int code)
{
    return -1000 - code;
}

int is_exit_status(int status)
{
    if (status > -1000)
        return 0;
    if (status < -1255)
        return 0;
    return 1;
}

int exit_status_code(int status)
{
    return -1000 - status;
}
