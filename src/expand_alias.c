/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** alias expansion
*/

#include "base.h"
#include "shell.h"
#include "my.h"

static int first_word_end(char *line)
{
    int i = 0;

    while (line[i] && line[i] != ' ' && line[i] != '\t')
        i++;
    return i;
}

static int is_visited(char visited[32][64], int count, char *word, int len)
{
    int i = 0;

    while (i < count) {
        if (my_strncmp(visited[i], word, len) == 0
            && visited[i][len] == '\0')
            return 1;
        i++;
    }
    return 0;
}

static char *lookup_alias(char *line, int end, char **aliases)
{
    char saved = line[end];
    char *val = NULL;

    line[end] = '\0';
    val = get_alias(aliases, line);
    line[end] = saved;
    return val;
}

static void mark_visited(char visited[32][64], int *count, char *word,
    int len)
{
    int copy_len = len < 63 ? len : 63;
    int i = 0;

    while (i < copy_len) {
        visited[*count][i] = word[i];
        i++;
    }
    visited[*count][copy_len] = '\0';
    (*count)++;
}

static char *try_expand(char *line, char **aliases, char visited[32][64],
    int *count)
{
    int end = first_word_end(line);
    char *val = NULL;

    if (end == 0 || *count >= 32)
        return NULL;
    if (is_visited(visited, *count, line, end))
        return NULL;
    val = lookup_alias(line, end, aliases);
    if (val == NULL)
        return NULL;
    mark_visited(visited, count, line, end);
    return my_strcat(val, line + end);
}

char *expand_alias_in_line(char *line, char **aliases)
{
    char visited[32][64] = {0};
    int count = 0;
    char *current = my_strdup(line);
    char *next = NULL;

    if (current == NULL || aliases == NULL)
        return current;
    while (1) {
        next = try_expand(current, aliases, visited, &count);
        if (next == NULL)
            break;
        free(current);
        current = next;
    }
    return current;
}
