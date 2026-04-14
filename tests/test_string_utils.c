/*
** EPITECH PROJECT, 2026
** test_string_utils
** File description:
** Tests pour les fonctions de manipulation de chaînes
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "base.h"
#include "my.h"

Test(my_strlen, empty_string)
{
    cr_assert_eq(my_strlen(""), 0);
}

Test(my_strlen, simple_string)
{
    cr_assert_eq(my_strlen("hello"), 5);
}

Test(my_strlen, long_string)
{
    cr_assert_eq(my_strlen("epitech school"), 14);
}

Test(my_strlen, with_spaces)
{
    cr_assert_eq(my_strlen("hello world"), 11);
}

Test(my_strlen, single_char)
{
    cr_assert_eq(my_strlen("a"), 1);
}

Test(my_strcmp, equal_strings)
{
    cr_assert_eq(my_strcmp("test", "test"), 0);
}

Test(my_strcmp, different_strings)
{
    cr_assert_neq(my_strcmp("abc", "xyz"), 0);
}

Test(my_strcmp, first_smaller)
{
    cr_assert_lt(my_strcmp("abc", "xyz"), 0);
}

Test(my_strcmp, first_greater)
{
    cr_assert_gt(my_strcmp("xyz", "abc"), 0);
}

Test(my_strcmp, empty_vs_nonempty)
{
    cr_assert_lt(my_strcmp("", "abc"), 0);
}

Test(my_strcmp, both_empty)
{
    cr_assert_eq(my_strcmp("", ""), 0);
}

Test(my_strcmp, prefix_match)
{
    cr_assert_neq(my_strcmp("test", "testing"), 0);
    cr_assert_lt(my_strcmp("test", "testing"), 0);
}

Test(my_strncmp, equal_n_chars)
{
    cr_assert_eq(my_strncmp("hello", "hello", 3), 0);
}

Test(my_strncmp, partial_match)
{
    cr_assert_eq(my_strncmp("hello", "help", 3), 0);
}

Test(my_strncmp, different_in_range)
{
    cr_assert_neq(my_strncmp("hello", "help", 5), 0);
}

Test(my_strncmp, zero_chars)
{
    cr_assert_lt(my_strncmp("abc", "xyz", 0), 0);
}

Test(my_strncmp, single_char_compare)
{
    cr_assert_eq(my_strncmp("abc", "ayz", 1), 0);
    cr_assert_neq(my_strncmp("abc", "xyz", 1), 0);
}

Test(my_strdup, simple_dup)
{
    char *dup = my_strdup("test");
    cr_assert_not_null(dup);
    cr_assert_str_eq(dup, "test");
    free(dup);
}

Test(my_strdup, empty_string_dup)
{
    char *dup = my_strdup("");
    cr_assert_not_null(dup);
    cr_assert_str_eq(dup, "");
    free(dup);
}

Test(my_strdup, long_string_dup)
{
    char *original = "this is a very long string for testing";
    char *dup = my_strdup(original);
    cr_assert_not_null(dup);
    cr_assert_str_eq(dup, original);
    free(dup);
}

Test(my_strdup, with_special_chars)
{
    char *dup = my_strdup("hello\nworld\ttab");
    cr_assert_not_null(dup);
    cr_assert_str_eq(dup, "hello\nworld\ttab");
    free(dup);
}

Test(my_strndup, partial_dup)
{
    char *dup = my_strndup("hello world", 5);
    cr_assert_not_null(dup);
    cr_assert_str_eq(dup, "hello");
    free(dup);
}

Test(my_strndup, zero_chars)
{
    char *dup = my_strndup("test", 0);
    cr_assert_not_null(dup);
    cr_assert_str_eq(dup, "");
    free(dup);
}

Test(my_strndup, full_string)
{
    char *dup = my_strndup("test", 4);
    cr_assert_not_null(dup);
    cr_assert_str_eq(dup, "test");
    free(dup);
}

Test(my_strndup, n_larger_than_string)
{
    char *dup = my_strndup("hi", 10);
    cr_assert_not_null(dup);
    cr_assert_str_eq(dup, "hi");
    free(dup);
}

Test(my_strndup, null_input)
{
    char *dup = my_strndup(NULL, 3);
    cr_assert_null(dup);
}

Test(my_strcat, simple_concat)
{
    char *result = my_strcat("hello", " world");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "hello world");
    free(result);
}

Test(my_strcat, empty_first)
{
    char *result = my_strcat("", "test");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "test");
    free(result);
}

Test(my_strcat, empty_second)
{
    char *result = my_strcat("test", "");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "test");
    free(result);
}

Test(my_strcat, both_empty)
{
    char *result = my_strcat("", "");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "");
    free(result);
}

Test(my_strcat, multiple_words)
{
    char *result = my_strcat("one two", " three four");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "one two three four");
    free(result);
}

Test(my_strcat, null_dest)
{
    char *result = my_strcat(NULL, "test");
    cr_assert_null(result);
}

Test(my_strcat, null_src)
{
    char *result = my_strcat("test", NULL);
    cr_assert_null(result);
}
