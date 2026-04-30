/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** variable expansion
*/

#include "base.h"
#include "shell.h"
#include "my.h"

static int is_var_char(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9') || c == '_';
}

static char *lookup_env(char *name, char **env)
{
    int name_len = my_strlen(name);
    int i = 0;

    if (env == NULL)
        return NULL;
    while (env[i] != NULL) {
        if (my_strncmp(env[i], name, name_len) == 0
            && env[i][name_len] == '=')
            return env[i] + name_len + 1;
        i++;
    }
    return NULL;
}

static char *get_cwd_value(void)
{
    char *buf = malloc(4096);

    if (buf == NULL)
        return NULL;
    if (getcwd(buf, 4096) == NULL) {
        free(buf);
        return NULL;
    }
    return buf;
}

static char *get_var_value(char *name, char **locals, char **env)
{
    char *val = NULL;

    if (my_strcmp(name, "cwd") == 0)
        return get_cwd_value();
    val = get_local(locals, name);
    if (val != NULL)
        return my_strdup(val);
    val = lookup_env(name, env);
    if (val != NULL)
        return my_strdup(val);
    return my_strdup("");
}

static char *str_append(char *dest, char *src)
{
    char *result = my_strcat(dest, src);

    free(dest);
    free(src);
    return result;
}

static char *consume_var(char *line, int *i, char **locals, char **env)
{
    int start = *i + 1;
    int end = start;
    char saved = '\0';
    char *val = NULL;

    while (line[end] && is_var_char(line[end]))
        end++;
    saved = line[end];
    line[end] = '\0';
    val = get_var_value(line + start, locals, env);
    line[end] = saved;
    *i = end;
    return val;
}

static char *consume_literal(char *line, int *i)
{
    int start = *i;

    while (line[*i] && line[*i] != '$')
        (*i)++;
    return my_strndup(line + start, *i - start);
}

char *expand_vars_in_line(char *line, char **locals, char **env)
{
    char *result = my_strdup("");
    char *chunk = NULL;
    int i = 0;

    while (result != NULL && line[i]) {
        if (line[i] == '$')
            chunk = consume_var(line, &i, locals, env);
        else
            chunk = consume_literal(line, &i);
        if (chunk == NULL) {
            free(result);
            return NULL;
        }
        result = str_append(result, chunk);
    }
    return result;
}
