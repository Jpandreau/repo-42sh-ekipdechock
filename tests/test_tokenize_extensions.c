/*
** EPITECH PROJECT, 2026
** test_tokenize_extensions
** File description:
** Tests for extended tokenizer features (globbing, inhibitors, substitutions)
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "base.h"
#include "tokenize_extensions.h"

Test(inhibitor, single_quote_inhibits_all)
{
    /* Inside single quotes, everything except the quote is literal */
    cr_assert_eq(is_inhibitor_char('$', '\''), 1);
    cr_assert_eq(is_inhibitor_char('`', '\''), 1);
    cr_assert_eq(is_inhibitor_char('\\', '\''), 1);
    cr_assert_eq(is_inhibitor_char('*', '\''), 1);
    cr_assert_eq(is_inhibitor_char('\'', '\''), 0); /* quote itself is not inhibited */
}

Test(inhibitor, double_quote_allows_special)
{
    /* Inside double quotes, $, `, \, and " remain special */
    cr_assert_eq(is_inhibitor_char('$', '"'), 0);  /* NOT inhibited */
    cr_assert_eq(is_inhibitor_char('`', '"'), 0);  /* NOT inhibited */
    cr_assert_eq(is_inhibitor_char('\\', '"'), 0); /* NOT inhibited */
    cr_assert_eq(is_inhibitor_char('"', '"'), 0);  /* NOT inhibited */
    cr_assert_eq(is_inhibitor_char('*', '"'), 1);  /* inhibited */
}

Test(inhibitor, no_quote_nothing_inhibited)
{
    /* Outside quotes, no inhibition */
    cr_assert_eq(is_inhibitor_char('$', 0), 0);
    cr_assert_eq(is_inhibitor_char('*', 0), 0);
    cr_assert_eq(is_inhibitor_char('`', 0), 0);
}

Test(glob_pattern, star_is_glob)
{
    cr_assert_eq(is_glob_pattern("*.c"), 1);
    cr_assert_eq(is_glob_pattern("src/*"), 1);
}

Test(glob_pattern, question_is_glob)
{
    cr_assert_eq(is_glob_pattern("test?.c"), 1);
    cr_assert_eq(is_glob_pattern("file_?"), 1);
}

Test(glob_pattern, bracket_is_glob)
{
    cr_assert_eq(is_glob_pattern("[abc]"), 1);
    cr_assert_eq(is_glob_pattern("file[0-9].txt"), 1);
}

Test(glob_pattern, no_glob_pattern)
{
    cr_assert_eq(is_glob_pattern("hello.txt"), 0);
    cr_assert_eq(is_glob_pattern("simple"), 0);
    cr_assert_eq(is_glob_pattern("file_name"), 0);
}

Test(glob_pattern, null_string)
{
    cr_assert_eq(is_glob_pattern(NULL), 0);
}

Test(glob_pattern, empty_string)
{
    cr_assert_eq(is_glob_pattern(""), 0);
}

Test(glob_pattern, multiple_patterns)
{
    cr_assert_eq(is_glob_pattern("*.[ch]"), 1);
    cr_assert_eq(is_glob_pattern("test_[0-9]*"), 1);
}

Test(expand_glob, c_files_expansion)
{
    char **results = NULL;
    int ret;

    ret = expand_glob_pattern("src/*.c", &results);
    cr_assert_eq(ret, 0);
    cr_assert_not_null(results);
    cr_assert_not_null(results[0]);
    free_array(results);
}

Test(expand_glob, h_files_expansion)
{
    char **results = NULL;
    int ret;

    ret = expand_glob_pattern("include/*.h", &results);
    cr_assert_eq(ret, 0);
    cr_assert_not_null(results);
    cr_assert_not_null(results[0]);
    free_array(results);
}

Test(expand_glob, no_match_returns_error)
{
    char **results = NULL;
    int ret;

    ret = expand_glob_pattern("nonexistent_*.xyz", &results);
    cr_assert_eq(ret, 1);
}

Test(expand_glob, null_pattern_returns_error)
{
    char **results = NULL;
    int ret;

    ret = expand_glob_pattern(NULL, &results);
    cr_assert_eq(ret, 1);
}

Test(expand_glob, null_results_returns_error)
{
    int ret;

    ret = expand_glob_pattern("src/*.c", NULL);
    cr_assert_eq(ret, 1);
}
