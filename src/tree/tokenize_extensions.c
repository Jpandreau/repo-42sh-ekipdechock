/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** Implementation of extended tokenizer features
*/

#include "base.h"
#include "tokenize_extensions.h"
#include <glob.h>

int is_inhibitor_char(char c, char quote)
{
    if (quote == '\'')
        return c != '\'';
    if (quote == '"')
        return c != '"' && c != '$' && c != '`' && c != '\\';
    return 0;
}

int is_glob_pattern(char *token)
{
    int i = 0;

    if (token == NULL)
        return 0;
    while (token[i]) {
        if (token[i] == '*' || token[i] == '?' || token[i] == '[')
            return 1;
        i++;
    }
    return 0;
}

static void free_partial_results(char **results, int count)
{
    int i = 0;

    while (i < count) {
        free(results[i]);
        i++;
    }
    free(results);
}

static int copy_glob_results(glob_t *globbuf, char ***results)
{
    char **expanded = NULL;
    int i = 0;

    expanded = malloc(sizeof(char *) * (globbuf->gl_pathc + 1));
    if (expanded == NULL)
        return 1;
    while (i < (int)globbuf->gl_pathc) {
        expanded[i] = my_strdup(globbuf->gl_pathv[i]);
        if (expanded[i] == NULL) {
            free_partial_results(expanded, i);
            return 1;
        }
        i++;
    }
    expanded[i] = NULL;
    *results = expanded;
    return 0;
}

int expand_glob_pattern(char *pattern, char ***results)
{
    glob_t globbuf;

    if (pattern == NULL || results == NULL)
        return 1;
    if (glob(pattern, 0, NULL, &globbuf) != 0)
        return 1;
    if (globbuf.gl_pathc == 0) {
        globfree(&globbuf);
        return 1;
    }
    if (copy_glob_results(&globbuf, results)) {
        globfree(&globbuf);
        return 1;
    }
    globfree(&globbuf);
    return 0;
}
