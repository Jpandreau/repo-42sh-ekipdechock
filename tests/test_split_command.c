/*
** EPITECH PROJECT, 2026
** test_split_command
** File description:
** Tests pour split_command
*/

#include <criterion/criterion.h>
#include "base.h"

char *select_args(char *input_line);
void split_command(char *input_line, char **line);

Test(select_args, simple_args)
{
    char *args = select_args("   -l -a");

    cr_assert_not_null(args);
    cr_assert_str_eq(args, "-l -a");
    free(args);
}

Test(select_args, only_spaces)
{
    char *args = select_args("   \t  ");

    cr_assert_null(args);
}

Test(split_command, command_and_args)
{
    char *line[2] = {NULL, NULL};

    split_command("ls -l", line);
    cr_assert_str_eq(line[0], "ls");
    cr_assert_str_eq(line[1], "-l");
    free(line[0]);
    free(line[1]);
}

Test(split_command, command_only)
{
    char *line[2] = {NULL, NULL};

    split_command("  pwd", line);
    cr_assert_str_eq(line[0], "pwd");
    cr_assert_null(line[1]);
    free(line[0]);
}

Test(split_command, leading_tabs)
{
    char *line[2] = {NULL, NULL};

    split_command("\t\tcd /tmp", line);
    cr_assert_str_eq(line[0], "cd");
    cr_assert_str_eq(line[1], "/tmp");
    free(line[0]);
    free(line[1]);
}

Test(split_command, empty_input)
{
    char *line[2] = {NULL, NULL};

    split_command("   ", line);
    cr_assert_str_eq(line[0], "");
    cr_assert_null(line[1]);
    free(line[0]);
}

Test(split_command, reuse_line_buffers)
{
    char *line[2] = {NULL, NULL};

    split_command("echo test", line);
    split_command("ls -a", line);
    cr_assert_str_eq(line[0], "ls");
    cr_assert_str_eq(line[1], "-a");
    free(line[0]);
    free(line[1]);
}
