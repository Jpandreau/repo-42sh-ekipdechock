/*
** EPITECH PROJECT, 2026
** test_exit_parse
** File description:
** Tests pour le parsing de exit
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "base.h"
#include "small_headers.h"

Test(parse_exit_code_arg, valid_positive)
{
    int ok = 0;
    int code = parse_exit_code_arg("42", &ok);

    cr_assert_eq(ok, 1);
    cr_assert_eq(code, 42);
}

Test(parse_exit_code_arg, valid_negative)
{
    int ok = 0;
    int code = parse_exit_code_arg("-5", &ok);

    cr_assert_eq(ok, 1);
    cr_assert_eq(code, 251);
}

Test(parse_exit_code_arg, valid_plus)
{
    int ok = 0;
    int code = parse_exit_code_arg("+7", &ok);

    cr_assert_eq(ok, 1);
    cr_assert_eq(code, 7);
}

Test(parse_exit_code_arg, invalid_alpha)
{
    int ok = 0;
    int code = parse_exit_code_arg("12a", &ok);

    cr_assert_eq(ok, 0);
    cr_assert_eq(code, 0);
}

Test(parse_exit_code_arg, invalid_empty)
{
    int ok = 0;
    int code = parse_exit_code_arg("", &ok);

    cr_assert_eq(ok, 0);
    cr_assert_eq(code, 0);
}

Test(exit_code_from_args, null_args)
{
    int valid = 1;
    int code = exit_code_from_args(NULL, &valid);

    cr_assert_eq(valid, 0);
    cr_assert_eq(code, 0);
}

Test(exit_code_from_args, no_arg)
{
    int valid = 0;
    char *args[] = {"exit", NULL};
    int code = exit_code_from_args(args, &valid);

    cr_assert_eq(valid, 1);
    cr_assert_eq(code, 0);
}

Test(exit_code_from_args, single_arg)
{
    int valid = 0;
    char *args[] = {"exit", "12", NULL};
    int code = exit_code_from_args(args, &valid);

    cr_assert_eq(valid, 1);
    cr_assert_eq(code, 12);
}

Test(exit_code_from_args, too_many_args)
{
    int valid = 1;
    char *args[] = {"exit", "12", "extra", NULL};
    int code = exit_code_from_args(args, &valid);

    cr_assert_eq(valid, 0);
    cr_assert_eq(code, 0);
}

Test(parse_exit_line, not_exit)
{
    int code = 0;
    int ret = parse_exit_line("echo test", &code);

    cr_assert_eq(ret, 0);
}

Test(parse_exit_line, exit_no_arg)
{
    int code = 42;
    int ret = parse_exit_line("exit", &code);

    cr_assert_eq(ret, 1);
    cr_assert_eq(code, 0);
}

Test(parse_exit_line, exit_with_spaces)
{
    int code = 0;
    int ret = parse_exit_line("  exit   7", &code);

    cr_assert_eq(ret, 1);
    cr_assert_eq(code, 7);
}

Test(parse_exit_line, exit_invalid_extra, .init = cr_redirect_stderr)
{
    int code = 0;
    int ret = parse_exit_line("exit 1 2", &code);

    cr_assert_eq(ret, 84);
    fflush(stderr);
    cr_assert_stderr_eq_str("exit: Expression Syntax.\n");
}

Test(parse_exit_line, exit_invalid_arg, .init = cr_redirect_stderr)
{
    int code = 0;
    int ret = parse_exit_line("exit abc", &code);

    cr_assert_eq(ret, 84);
    fflush(stderr);
    cr_assert_stderr_eq_str("exit: Expression Syntax.\n");
}
