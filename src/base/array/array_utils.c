/*
** EPITECH PROJECT, 2026
** array_utils
** File description:
** array helpers
*/

#include "base.h"

int array_size(char **array)
{
    int size = 0;

    if (array == NULL)
        return 0;
    for (; array[size] != NULL; size++);
    return size;
}

int free_array(char **array)
{
    int i = 0;

    if (array == NULL)
        return 84;
    for (; array[i] != NULL; i++) {
        if (array[i] == NULL)
            continue;
        free(array[i]);
    }
    free(array);
    return 84;
}

void free_line(char **line)
{
    if (line == NULL)
        return;
    if (line[0] != NULL)
        free(line[0]);
    if (line[1] != NULL)
        free(line[1]);
}

char **my_array_add(char **array, char *new_str)
{
    char **new_array = NULL;
    int size = 0;

    if (array == NULL || new_str == NULL)
        return NULL;
    size = array_size(array);
    new_array = malloc(sizeof(char *) * (size + 2));
    if (new_array == NULL)
        return NULL;
    for (int i = 0; i < size; i++)
        new_array[i] = array[i];
    new_array[size] = new_str;
    new_array[size + 1] = NULL;
    free(array);
    return new_array;
}

char **my_array_dup(char **array)
{
    char **new_array = NULL;
    int size = array_size(array);

    new_array = malloc(sizeof(char *) * (size + 1));
    if (new_array == NULL)
        return NULL;
    for (int i = 0; i < size; i++) {
        new_array[i] = my_strdup(array[i]);
        if (new_array[i] == NULL) {
            free_array(new_array);
            return NULL;
        }
    }
    new_array[size] = NULL;
    return new_array;
}
