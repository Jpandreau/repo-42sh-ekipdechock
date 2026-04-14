/*
** EPITECH PROJECT, 2026
** str_word_array
** File description:
** split string into word array
*/

#include "base.h"

static int get_array_size(char const *str, char const delim)
{
    int size = str[0] && !is_sep(str[0], delim);

    for (int i = 1; str[i] != '\0'; i++) {
        if (!is_sep(str[i], delim) && is_sep(str[i - 1], delim))
            size++;
    }
    return size;
}

static int fill_array(char **array, char const *str, char const delim)
{
    int index = 0;
    int start = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (is_sep(str[i], delim))
            continue;
        start = i;
        while (!is_sep(str[i], delim) && str[i] != '\0')
            i++;
        array[index] = my_strndup(str + start, i - start);
        if (array[index] == NULL) {
            free_array(array);
            return 1;
        }
        index++;
        if (str[i] == '\0')
            break;
    }
    return 0;
}

char **my_str_to_word_array(char const *str, char const delim)
{
    int array_size = 0;
    int i = 0;
    char **array = NULL;

    if (str == NULL)
        return NULL;
    array_size = get_array_size(str, delim);
    array = malloc(sizeof(char *) * (array_size + 1));
    if (array == NULL)
        return NULL;
    for (; i <= array_size; i++)
        array[i] = NULL;
    if (array_size == 0)
        return array;
    if (fill_array(array, str, delim))
        return NULL;
    return array;
}
