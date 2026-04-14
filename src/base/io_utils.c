/*
** EPITECH PROJECT, 2026
** io_utils
** File description:
** io utils
*/

#include "base.h"

void my_putchar(char c)
{
    write(1, &c, 1);
}

void my_putstr(char const *str)
{
    write(1, str, my_strlen(str));
}

void my_putstr_err(char const *str)
{
    write(2, str, my_strlen(str));
}
