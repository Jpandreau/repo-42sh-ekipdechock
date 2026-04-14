/*
** EPITECH PROJECT, 2026
** test_io_utils
** File description:
** Tests pour les fonctions d'affichage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "base.h"

Test(my_putchar, writes_one_char, .init = cr_redirect_stdout)
{
    my_putchar('A');
    fflush(stdout);
    cr_assert_stdout_eq_str("A");
}

Test(my_putstr, writes_string, .init = cr_redirect_stdout)
{
    my_putstr("hello");
    fflush(stdout);
    cr_assert_stdout_eq_str("hello");
}

Test(my_putstr, writes_empty, .init = cr_redirect_stdout)
{
    my_putstr("");
    fflush(stdout);
    cr_assert_stdout_eq_str("");
}

Test(my_putstr_err, writes_error, .init = cr_redirect_stderr)
{
    my_putstr_err("error");
    fflush(stderr);
    cr_assert_stderr_eq_str("error");
}
