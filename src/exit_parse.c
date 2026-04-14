/*
** EPITECH PROJECT, 2026
** exit_parse
** File description:
** exit_parse
*/

#include "small_headers.h"
#include "base.h"

static int is_space(char c)
{
    return (c == ' ' || c == '\t');
}

static int has_shell_operator(char *line)
{
    for (int i = 0; line[i] != '\0'; i++)
        if (line[i] == '|' || line[i] == ';' || line[i] == '<' ||
            line[i] == '>' || line[i] == '&')
            return 1;
    return 0;
}

static int skip_spaces(char *line)
{
    int i = 0;

    while (line[i] && is_space(line[i]))
        i++;
    return i;
}

static int exit_syntax(void)
{
    my_putstr_err("exit: Expression Syntax.\n");
    return 84;
}

static int is_exit_word(char *line, int *pos)
{
    int i = skip_spaces(line);

    if (my_strncmp(line + i, "exit", 4) != 0)
        return 0;
    if (line[i + 4] != '\0' && !is_space(line[i + 4]))
        return 0;
    *pos = i + 4;
    return 1;
}

static int parse_exit_arg(char *line, int pos, int *code)
{
    int ok = 0;
    int start = pos + skip_spaces(line + pos);
    int i = start;

    if (line[start] == '\0') {
        *code = 0;
        return 1;
    }
    while (line[i] && !is_space(line[i]))
        i++;
    if (line[i] != '\0')
        return exit_syntax();
    *code = parse_exit_code_arg(line + start, &ok);
    if (!ok)
        return exit_syntax();
    return 1;
}

int parse_exit_line(char *line, int *code)
{
    int pos = 0;

    if (has_shell_operator(line))
        return 0;
    if (!is_exit_word(line, &pos))
        return 0;
    return parse_exit_arg(line, pos, code);
}
