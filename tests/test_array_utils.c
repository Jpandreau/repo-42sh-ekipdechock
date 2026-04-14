/*
** EPITECH PROJECT, 2026
** test_array_utils
** File description:
** Tests pour les fonctions de manipulation d'arrays
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "base.h"
#include "my.h"

Test(array_size, empty_array)
{
    char *arr[] = {NULL};
    cr_assert_eq(array_size(arr), 0);
}

Test(array_size, single_element)
{
    char *arr[] = {"one", NULL};
    cr_assert_eq(array_size(arr), 1);
}

Test(array_size, multiple_elements)
{
    char *arr[] = {"one", "two", "three", NULL};
    cr_assert_eq(array_size(arr), 3);
}

Test(array_size, many_elements)
{
    char *arr[] = {"a", "b", "c", "d", "e", "f", "g", NULL};
    cr_assert_eq(array_size(arr), 7);
}

Test(array_size, with_empty_strings)
{
    char *arr[] = {"", "test", "", NULL};
    cr_assert_eq(array_size(arr), 3);
}

Test(array_size, null_array)
{
    cr_assert_eq(array_size(NULL), 0);
}

Test(my_array_dup, simple_array)
{
    char *arr[] = {"one", "two", "three", NULL};
    char **dup = my_array_dup(arr);
    cr_assert_not_null(dup);
    cr_assert_str_eq(dup[0], "one");
    cr_assert_str_eq(dup[1], "two");
    cr_assert_str_eq(dup[2], "three");
    cr_assert_null(dup[3]);
    free_array(dup);
}

Test(my_array_dup, empty_array)
{
    char *arr[] = {NULL};
    char **dup = my_array_dup(arr);
    cr_assert_not_null(dup);
    cr_assert_null(dup[0]);
    free_array(dup);
}

Test(my_array_dup, single_element_array)
{
    char *arr[] = {"only", NULL};
    char **dup = my_array_dup(arr);
    cr_assert_not_null(dup);
    cr_assert_str_eq(dup[0], "only");
    cr_assert_null(dup[1]);
    free_array(dup);
}

Test(my_array_dup, long_strings)
{
    char *arr[] = {"this is a very long string", "another long one", NULL};
    char **dup = my_array_dup(arr);
    cr_assert_not_null(dup);
    cr_assert_str_eq(dup[0], "this is a very long string");
    cr_assert_str_eq(dup[1], "another long one");
    cr_assert_null(dup[2]);
    free_array(dup);
}

Test(my_array_dup, with_empty_strings)
{
    char *arr[] = {"", "test", "", NULL};
    char **dup = my_array_dup(arr);
    cr_assert_not_null(dup);
    cr_assert_str_eq(dup[0], "");
    cr_assert_str_eq(dup[1], "test");
    cr_assert_str_eq(dup[2], "");
    cr_assert_null(dup[3]);
    free_array(dup);
}

Test(free_array, simple_free)
{
    char **arr = malloc(sizeof(char *) * 3);
    arr[0] = my_strdup("one");
    arr[1] = my_strdup("two");
    arr[2] = NULL;
    free_array(arr);
    cr_assert(1);
}

Test(free_array, empty_array)
{
    char **arr = malloc(sizeof(char *) * 1);
    arr[0] = NULL;
    free_array(arr);
    cr_assert(1);
}

Test(free_array, null_array)
{
    cr_assert_eq(free_array(NULL), 84);
}

Test(free_array, large_array)
{
    char **arr;
    int i;

    arr = malloc(sizeof(char *) * 11);
    for (i = 0; i < 10; i++)
        arr[i] = my_strdup("test");
    arr[10] = NULL;
    free_array(arr);
    cr_assert(1);
}

Test(free_line, both_set)
{
    char *line[2];

    line[0] = my_strdup("cmd");
    line[1] = my_strdup("arg");
    free_line(line);
    cr_assert(1);
}

Test(free_line, null_entries)
{
    char *line[2];

    line[0] = NULL;
    line[1] = NULL;
    free_line(line);
    cr_assert(1);
}

Test(my_array_add, add_to_empty)
{
    char **arr;
    char **new_arr;

    arr = malloc(sizeof(char *) * 1);
    arr[0] = NULL;
    new_arr = my_array_add(arr, my_strdup("one"));
    cr_assert_not_null(new_arr);
    cr_assert_str_eq(new_arr[0], "one");
    cr_assert_null(new_arr[1]);
    free_array(new_arr);
}

Test(my_array_add, add_to_existing)
{
    char **arr;
    char **new_arr;

    arr = malloc(sizeof(char *) * 2);
    arr[0] = my_strdup("one");
    arr[1] = NULL;
    new_arr = my_array_add(arr, my_strdup("two"));
    cr_assert_not_null(new_arr);
    cr_assert_str_eq(new_arr[0], "one");
    cr_assert_str_eq(new_arr[1], "two");
    cr_assert_null(new_arr[2]);
    free_array(new_arr);
}

Test(my_array_add, null_array)
{
    char **new_arr = my_array_add(NULL, my_strdup("one"));
    cr_assert_null(new_arr);
}

Test(my_array_add, null_string)
{
    char **arr;
    char **new_arr;

    arr = malloc(sizeof(char *) * 1);
    arr[0] = NULL;
    new_arr = my_array_add(arr, NULL);
    cr_assert_null(new_arr);
    free(arr);
}
