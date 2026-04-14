/*
** EPITECH PROJECT, 2026
** test_malloc_failures
** File description:
** malloc failure coverage for isolated helpers
*/

#include <criterion/criterion.h>
#include <stdlib.h>

static void *failing_malloc(size_t size)
{
    (void)size;
    return NULL;
}

#define malloc failing_malloc
#define my_strdup tested_my_strdup_fail
#define my_strndup tested_my_strndup_fail
#define my_strcat tested_my_strcat_fail
#include "../src/base/str/str_dup.c"
#undef my_strcat
#undef my_strndup
#undef my_strdup
#undef malloc

char *my_strndup(char const *str, int n);

#define malloc failing_malloc
#define my_str_to_word_array tested_my_str_to_word_array_fail
#include "../src/base/str/str_word_array.c"
#undef my_str_to_word_array
#undef malloc

Test(malloc_failures, strdup_returns_null)
{
    cr_assert_null(tested_my_strdup_fail("hello"));
}

Test(malloc_failures, strndup_returns_null)
{
    cr_assert_null(tested_my_strndup_fail("hello", 3));
}

Test(malloc_failures, strcat_returns_null)
{
    cr_assert_null(tested_my_strcat_fail("ab", "cd"));
}

Test(malloc_failures, word_array_returns_null)
{
    cr_assert_null(tested_my_str_to_word_array_fail("a b", ' '));
}
