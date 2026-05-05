/*
** EPITECH PROJECT, 2026
** handle_sigint
** File description:
** handle_sigint
*/

#include "my.h"

void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\r\n$> ", 5);
}
