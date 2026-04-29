/*
** EPITECH PROJECT, 2026
** test_set_unset
** File description:
** Tests Criterion pour les variables locales (set/unset)
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "base.h"
#include "shell.h"

static char **make_locals(void)
{
    char **l = malloc(sizeof(char *));

    l[0] = NULL;
    return l;
}

/* ---- set_local ---- */

Test(set_local, create_new)
{
    char **locals = make_locals();
    int ret = set_local(&locals, "myvar", "hello");

    cr_assert_eq(ret, 0);
    cr_assert_not_null(locals);
    cr_assert_str_eq(locals[0], "myvar=hello");
    free(locals[0]);
    free(locals);
}

Test(set_local, overwrite_existing)
{
    char **locals = make_locals();

    set_local(&locals, "myvar", "hello");
    int ret = set_local(&locals, "myvar", "world");

    cr_assert_eq(ret, 0);
    cr_assert_str_eq(locals[0], "myvar=world");
    cr_assert_null(locals[1]);
    free(locals[0]);
    free(locals);
}

Test(set_local, create_multiple)
{
    char **locals = make_locals();

    set_local(&locals, "foo", "1");
    set_local(&locals, "bar", "2");
    cr_assert_str_eq(locals[0], "foo=1");
    cr_assert_str_eq(locals[1], "bar=2");
    cr_assert_null(locals[2]);
    free(locals[0]);
    free(locals[1]);
    free(locals);
}

Test(set_local, empty_value)
{
    char **locals = make_locals();
    int ret = set_local(&locals, "empty", "");

    cr_assert_eq(ret, 0);
    cr_assert_str_eq(locals[0], "empty=");
    free(locals[0]);
    free(locals);
}

/* ---- get_local ---- */

Test(get_local, find_existing)
{
    char **locals = make_locals();

    set_local(&locals, "myvar", "hello");
    char *val = get_local(locals, "myvar");

    cr_assert_not_null(val);
    cr_assert_str_eq(val, "hello");
    free(locals[0]);
    free(locals);
}

Test(get_local, null_on_missing)
{
    char **locals = make_locals();

    set_local(&locals, "foo", "1");
    cr_assert_null(get_local(locals, "bar"));
    free(locals[0]);
    free(locals);
}

Test(get_local, null_on_empty_table)
{
    char **locals = make_locals();

    cr_assert_null(get_local(locals, "anything"));
    free(locals);
}

Test(get_local, no_partial_match)
{
    char **locals = make_locals();

    set_local(&locals, "myvar", "hello");
    cr_assert_null(get_local(locals, "my"));
    free(locals[0]);
    free(locals);
}

Test(get_local, find_second_entry)
{
    char **locals = make_locals();

    set_local(&locals, "a", "1");
    set_local(&locals, "b", "2");
    cr_assert_str_eq(get_local(locals, "b"), "2");
    free(locals[0]);
    free(locals[1]);
    free(locals);
}

/* ---- unset_local ---- */

Test(unset_local, remove_existing)
{
    char **locals = make_locals();

    set_local(&locals, "myvar", "hello");
    unset_local(&locals, "myvar");
    cr_assert_null(get_local(locals, "myvar"));
    free(locals);
}

Test(unset_local, remove_nonexistent_no_crash)
{
    char **locals = make_locals();
    int ret = unset_local(&locals, "nope");

    cr_assert_eq(ret, 0);
    free(locals);
}

Test(unset_local, keep_others_after_remove)
{
    char **locals = make_locals();

    set_local(&locals, "foo", "1");
    set_local(&locals, "bar", "2");
    unset_local(&locals, "foo");
    cr_assert_null(get_local(locals, "foo"));
    cr_assert_str_eq(get_local(locals, "bar"), "2");
    free(locals[0]);
    free(locals);
}

/* ---- set_builtin_args ---- */

Test(set_builtin_args, no_args_display_empty, .init = cr_redirect_stdout)
{
    char **locals = make_locals();
    char *args[] = {"set", NULL};
    int ret = set_builtin_args(args, &locals);

    cr_assert_eq(ret, 0);
    fflush(stdout);
    cr_assert_stdout_eq_str("");
    free(locals);
}

Test(set_builtin_args, create_variable)
{
    char **locals = make_locals();
    char *args[] = {"set", "myvar", "hello", NULL};
    int ret = set_builtin_args(args, &locals);

    cr_assert_eq(ret, 0);
    cr_assert_str_eq(get_local(locals, "myvar"), "hello");
    free(locals[0]);
    free(locals);
}

Test(set_builtin_args, overwrite_variable)
{
    char **locals = make_locals();
    char *args1[] = {"set", "myvar", "hello", NULL};
    char *args2[] = {"set", "myvar", "world", NULL};

    set_builtin_args(args1, &locals);
    set_builtin_args(args2, &locals);
    cr_assert_str_eq(get_local(locals, "myvar"), "world");
    free(locals[0]);
    free(locals);
}

/* ---- unset_builtin_args ---- */

Test(unset_builtin_args, too_few_args, .init = cr_redirect_stderr)
{
    char **locals = make_locals();
    char *args[] = {"unset", NULL};
    int ret = unset_builtin_args(args, &locals);

    cr_assert_neq(ret, 0);
    free(locals);
}

Test(unset_builtin_args, remove_variable)
{
    char **locals = make_locals();
    char *args_set[] = {"set", "myvar", "hello", NULL};
    char *args_unset[] = {"unset", "myvar", NULL};

    set_builtin_args(args_set, &locals);
    unset_builtin_args(args_unset, &locals);
    cr_assert_null(get_local(locals, "myvar"));
    free(locals);
}
