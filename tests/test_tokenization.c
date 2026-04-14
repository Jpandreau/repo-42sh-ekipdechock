/*
** EPITECH PROJECT, 2026
** test_tokenization
** File description:
** Tests pour la tokenization et le parsing de lignes
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "base.h"
#include "my.h"
#include "tokenize_line_helpers.h"

Test(is_sep, space_is_sep)
{
    cr_assert_eq(is_sep(' ', ' '), 1);
}

Test(is_sep, tab_is_sep_for_space)
{
    cr_assert_eq(is_sep('\t', ' '), 1);
}

Test(is_sep, letter_not_sep)
{
    cr_assert_eq(is_sep('a', ' '), 0);
}

Test(is_sep, custom_delimiter)
{
    cr_assert_eq(is_sep(',', ','), 1);
    cr_assert_eq(is_sep('a', ','), 0);
}

Test(is_sep, newline_not_sep_for_space)
{
    cr_assert_eq(is_sep('\n', ' '), 0);
}

Test(is_sep, colon_delimiter)
{
    cr_assert_eq(is_sep(':', ':'), 1);
    cr_assert_eq(is_sep(' ', ':'), 0);
}

Test(my_str_to_word_array, simple_split)
{
    char **result = my_str_to_word_array("one two three", ' ');
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "one");
    cr_assert_str_eq(result[1], "two");
    cr_assert_str_eq(result[2], "three");
    cr_assert_null(result[3]);
    free_array(result);
}

Test(my_str_to_word_array, single_word)
{
    char **result = my_str_to_word_array("hello", ' ');
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "hello");
    cr_assert_null(result[1]);
    free_array(result);
}

Test(my_str_to_word_array, multiple_spaces)
{
    char **result = my_str_to_word_array("one  two   three", ' ');
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "one");
    cr_assert_str_eq(result[1], "two");
    cr_assert_str_eq(result[2], "three");
    cr_assert_null(result[3]);
    free_array(result);
}

Test(my_str_to_word_array, leading_trailing_spaces)
{
    char **result = my_str_to_word_array("  one two  ", ' ');
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "one");
    cr_assert_str_eq(result[1], "two");
    cr_assert_null(result[2]);
    free_array(result);
}

Test(my_str_to_word_array, empty_string)
{
    char **result = my_str_to_word_array("", ' ');
    cr_assert_not_null(result);
    cr_assert_null(result[0]);
    free_array(result);
}

Test(my_str_to_word_array, null_input)
{
    char **result = my_str_to_word_array(NULL, ' ');
    cr_assert_null(result);
}

Test(my_str_to_word_array, only_spaces)
{
    char **result = my_str_to_word_array("     ", ' ');
    cr_assert_not_null(result);
    cr_assert_null(result[0]);
    free_array(result);
}

Test(my_str_to_word_array, with_tabs)
{
    char **result = my_str_to_word_array("one\ttwo\tthree", ' ');
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "one");
    cr_assert_str_eq(result[1], "two");
    cr_assert_str_eq(result[2], "three");
    cr_assert_null(result[3]);
    free_array(result);
}

Test(my_str_to_word_array, colon_separator)
{
    char **result = my_str_to_word_array("/bin:/usr/bin:/usr/local/bin", ':');
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "/bin");
    cr_assert_str_eq(result[1], "/usr/bin");
    cr_assert_str_eq(result[2], "/usr/local/bin");
    cr_assert_null(result[3]);
    free_array(result);
}

Test(my_str_to_word_array, comma_separator)
{
    char **result = my_str_to_word_array("a,b,c,d", ',');
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "a");
    cr_assert_str_eq(result[1], "b");
    cr_assert_str_eq(result[2], "c");
    cr_assert_str_eq(result[3], "d");
    cr_assert_null(result[4]);
    free_array(result);
}

Test(my_str_to_word_array, mixed_separators)
{
    char **result = my_str_to_word_array("a  \tb   \t  c", ' ');
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "a");
    cr_assert_str_eq(result[1], "b");
    cr_assert_str_eq(result[2], "c");
    cr_assert_null(result[3]);
    free_array(result);
}
