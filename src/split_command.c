/*
** EPITECH PROJECT, 2026
** split_command
** File description:
** split_command
*/

#include "my.h"
#include "base.h"

char *select_args(char *input_line)
{
    int size = 0;

    for (; input_line[size] &&
        (input_line[size] == ' ' || input_line[size] == '\t'); size++);
    if (input_line[size] == '\0')
        return NULL;
    return my_strdup(input_line + size);
}

void split_command(char *input_line, char **line)
{
    int size = 0;

    for (; *input_line &&
        (*input_line == ' ' || *input_line == '\t'); input_line++);
    for (; input_line[size] &&
        (input_line[size] != ' ' && input_line[size] != '\t'); size++);
    if (line[0] != NULL)
        free(line[0]);
    line[0] = my_strndup(input_line, size);
    if (line[0] == NULL)
        return;
    if (line[1] != NULL)
        free(line[1]);
    if (input_line[size] == '\0') {
        line[1] = NULL;
        return;
    }
    line[1] = select_args(input_line + size);
}
