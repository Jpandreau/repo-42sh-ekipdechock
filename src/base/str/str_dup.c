/*
** EPITECH PROJECT, 2026
** str_dup
** File description:
** string duplication helpers
*/

#include "base.h"

char *my_strdup(char const *str)
{
    char *new_str = NULL;
    int size;

    if (str == NULL)
        return NULL;
    size = my_strlen(str);
    new_str = malloc(sizeof(char) * (size + 1));
    if (new_str == NULL)
        return NULL;
    for (int i = 0; i < size; i++)
        new_str[i] = str[i];
    new_str[size] = '\0';
    return new_str;
}

char *my_strndup(char const *str, int n)
{
    char *new_str = NULL;

    if (str == NULL)
        return NULL;
    new_str = malloc(sizeof(char) * (n + 1));
    if (new_str == NULL)
        return NULL;
    for (int i = 0; i < n; i++)
        new_str[i] = str[i];
    new_str[n] = '\0';
    return new_str;
}

char *my_strcat(char *dest, char const *src)
{
    int size_dest = 0;
    int size_src = 0;
    char *new_str = NULL;

    if (dest == NULL || src == NULL)
        return NULL;
    size_dest = my_strlen(dest);
    size_src = my_strlen(src);
    new_str = malloc(sizeof(char) * (size_dest + size_src + 1));
    if (new_str == NULL)
        return NULL;
    for (int i = 0; i < size_dest; i++)
        new_str[i] = dest[i];
    for (int i = 0; i < size_src; i++)
        new_str[size_dest + i] = src[i];
    new_str[size_dest + size_src] = '\0';
    return new_str;
}
