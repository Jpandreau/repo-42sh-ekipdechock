/*
** EPITECH PROJECT, 2026
** str_basic
** File description:
** basic string helpers
*/

#include "base.h"

int my_strlen(char const *str)
{
    int size = 0;

    while (str[size])
        size++;
    return size;
}

int is_sep(char c, char delim)
{
    if (c == delim)
        return 1;
    if (delim == ' ' && c == '\t')
        return 1;
    return 0;
}

int my_strcmp(const char *s1, const char *s2)
{
    int index = 0;

    while (s1[index] && s2[index]) {
        if (s1[index] != s2[index])
            return (s1[index] - s2[index]);
        index++;
    }
    return (s1[index] - s2[index]);
}

int my_strncmp(char const *s1, char const *s2, int n)
{
    int index = 0;

    while (s1[index] && s2[index] && index < n - 1) {
        if (s1[index] != s2[index])
            return (s1[index] - s2[index]);
        index++;
    }
    return (s1[index] - s2[index]);
}
