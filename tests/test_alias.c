/*
** EPITECH PROJECT, 2026
** test_alias
** File description:
** Tests Criterion pour les fonctions d'alias
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "base.h"
#include "shell.h"

static char **make_aliases(void)
{
    char **a = malloc(sizeof(char *));

    a[0] = NULL;
    return a;
}

Test(set_alias_entry, create_new)
{
    char **aliases = make_aliases();
    int ret = set_alias_entry(&aliases, "ll", "ls");

    cr_assert_eq(ret, 0);
    cr_assert_not_null(aliases);
    cr_assert_str_eq(aliases[0], "ll=ls");
    free(aliases[0]);
    free(aliases);
}

Test(set_alias_entry, overwrite_existing)
{
    char **aliases = make_aliases();

    set_alias_entry(&aliases, "ll", "ls");
    int ret = set_alias_entry(&aliases, "ll", "ls -la");

    cr_assert_eq(ret, 0);
    cr_assert_str_eq(aliases[0], "ll=ls -la");
    cr_assert_null(aliases[1]);
    free(aliases[0]);
    free(aliases);
}

Test(set_alias_entry, create_multiple)
{
    char **aliases = make_aliases();

    set_alias_entry(&aliases, "ll", "ls");
    set_alias_entry(&aliases, "la", "ls -a");
    cr_assert_str_eq(aliases[0], "ll=ls");
    cr_assert_str_eq(aliases[1], "la=ls -a");
    cr_assert_null(aliases[2]);
    free(aliases[0]);
    free(aliases[1]);
    free(aliases);
}

Test(set_alias_entry, empty_value)
{
    char **aliases = make_aliases();
    int ret = set_alias_entry(&aliases, "foo", "");

    cr_assert_eq(ret, 0);
    cr_assert_str_eq(aliases[0], "foo=");
    free(aliases[0]);
    free(aliases);
}

Test(get_alias, find_existing)
{
    char **aliases = make_aliases();

    set_alias_entry(&aliases, "ll", "ls -la");
    char *val = get_alias(aliases, "ll");

    cr_assert_not_null(val);
    cr_assert_str_eq(val, "ls -la");
    free(aliases[0]);
    free(aliases);
}

Test(get_alias, null_on_missing)
{
    char **aliases = make_aliases();

    set_alias_entry(&aliases, "ll", "ls");
    cr_assert_null(get_alias(aliases, "la"));
    free(aliases[0]);
    free(aliases);
}

Test(get_alias, null_on_empty_table)
{
    char **aliases = make_aliases();

    cr_assert_null(get_alias(aliases, "ll"));
    free(aliases);
}

Test(get_alias, no_partial_match)
{
    char **aliases = make_aliases();

    set_alias_entry(&aliases, "ll", "ls");
    cr_assert_null(get_alias(aliases, "l"));
    free(aliases[0]);
    free(aliases);
}

Test(get_alias, find_second_entry)
{
    char **aliases = make_aliases();

    set_alias_entry(&aliases, "ll", "ls");
    set_alias_entry(&aliases, "la", "ls -a");
    cr_assert_str_eq(get_alias(aliases, "la"), "ls -a");
    free(aliases[0]);
    free(aliases[1]);
    free(aliases);
}

Test(unalias_builtin_args, too_few_args, .init = cr_redirect_stderr)
{
    char **aliases = make_aliases();
    char *args[] = {"unalias", NULL};
    int ret = unalias_builtin_args(args, &aliases);

    cr_assert_eq(ret, 1);
    free(aliases);
}

Test(unalias_builtin_args, remove_existing)
{
    char **aliases = make_aliases();
    char *args[] = {"unalias", "ll", NULL};

    set_alias_entry(&aliases, "ll", "ls");
    unalias_builtin_args(args, &aliases);
    cr_assert_null(get_alias(aliases, "ll"));
    free(aliases);
}

Test(unalias_builtin_args, remove_nonexistent_no_crash)
{
    char **aliases = make_aliases();
    char *args[] = {"unalias", "nope", NULL};
    int ret = unalias_builtin_args(args, &aliases);

    cr_assert_eq(ret, 0);
    free(aliases);
}

Test(unalias_builtin_args, remove_multiple)
{
    char **aliases = make_aliases();
    char *args[] = {"unalias", "ll", "la", NULL};

    set_alias_entry(&aliases, "ll", "ls");
    set_alias_entry(&aliases, "la", "ls -a");
    unalias_builtin_args(args, &aliases);
    cr_assert_null(get_alias(aliases, "ll"));
    cr_assert_null(get_alias(aliases, "la"));
    free(aliases);
}

Test(unalias_builtin_args, keep_others_after_remove)
{
    char **aliases = make_aliases();
    char *args[] = {"unalias", "ll", NULL};

    set_alias_entry(&aliases, "ll", "ls");
    set_alias_entry(&aliases, "la", "ls -a");
    unalias_builtin_args(args, &aliases);
    cr_assert_null(get_alias(aliases, "ll"));
    cr_assert_str_eq(get_alias(aliases, "la"), "ls -a");
    free(aliases[0]);
    free(aliases);
}

Test(alias_builtin_args, no_args_empty_display, .init = cr_redirect_stdout)
{
    char **aliases = make_aliases();
    char *args[] = {"alias", NULL};
    int ret = alias_builtin_args(args, &aliases);

    cr_assert_eq(ret, 0);
    fflush(stdout);
    cr_assert_stdout_eq_str("");
    free(aliases);
}

Test(alias_builtin_args, no_args_display_all, .init = cr_redirect_stdout)
{
    char **aliases = make_aliases();
    char *args[] = {"alias", NULL};

    set_alias_entry(&aliases, "ll", "ls");
    alias_builtin_args(args, &aliases);
    fflush(stdout);
    cr_assert_stdout_eq_str("ll=ls\n");
    free(aliases[0]);
    free(aliases);
}

Test(alias_builtin_args, two_args_display_one, .init = cr_redirect_stdout)
{
    char **aliases = make_aliases();
    char *args[] = {"alias", "ll", NULL};

    set_alias_entry(&aliases, "ll", "ls -la");
    alias_builtin_args(args, &aliases);
    fflush(stdout);
    cr_assert_stdout_eq_str("ls -la\n");
    free(aliases[0]);
    free(aliases);
}

Test(alias_builtin_args, three_args_creates)
{
    char **aliases = make_aliases();
    char *args[] = {"alias", "ll", "ls -la", NULL};
    int ret = alias_builtin_args(args, &aliases);

    cr_assert_eq(ret, 0);
    cr_assert_str_eq(get_alias(aliases, "ll"), "ls -la");
    free(aliases[0]);
    free(aliases);
}

Test(alias_builtin_args, three_args_overwrites, .init = cr_redirect_stdout)
{
    char **aliases = make_aliases();
    char *args_create[] = {"alias", "ll", "ls", NULL};
    char *args_update[] = {"alias", "ll", "ls -la", NULL};

    alias_builtin_args(args_create, &aliases);
    alias_builtin_args(args_update, &aliases);
    cr_assert_str_eq(get_alias(aliases, "ll"), "ls -la");
    free(aliases[0]);
    free(aliases);
}
