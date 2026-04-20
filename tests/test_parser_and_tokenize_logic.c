/*
** EPITECH PROJECT, 2026
** test_parser_and_tokenize_logic
** File description:
** direct tests for tokenize_line and parse_logic
*/

#include <criterion/criterion.h>
#include "small_headers.h"
#include "tree.h"
#include "base.h"

tree_type_t get_logic_type(char *token);

Test(tokenize_line, quoted_word_kept_together)
{
    char **tokens = tokenize_line("echo 'ab cd'");

    cr_assert_not_null(tokens);
    cr_assert_str_eq(tokens[0], "echo");
    cr_assert_str_eq(tokens[1], "ab cd");
    cr_assert_null(tokens[2]);
    free_array(tokens);
}

Test(tokenize_line, escaped_space_kept_in_token)
{
    char **tokens = tokenize_line("a\\ b");

    cr_assert_not_null(tokens);
    cr_assert_str_eq(tokens[0], "a b");
    cr_assert_null(tokens[1]);
    free_array(tokens);
}

Test(tokenize_line, operators_without_spaces)
{
    char **tokens = tokenize_line("a&&b||c>>d<<e;f");

    cr_assert_not_null(tokens);
    cr_assert_str_eq(tokens[0], "a");
    cr_assert_str_eq(tokens[1], "&&");
    cr_assert_str_eq(tokens[2], "b");
    cr_assert_str_eq(tokens[3], "||");
    cr_assert_str_eq(tokens[4], "c");
    cr_assert_str_eq(tokens[5], ">>");
    cr_assert_str_eq(tokens[6], "d");
    cr_assert_str_eq(tokens[7], "<<");
    cr_assert_str_eq(tokens[8], "e");
    cr_assert_str_eq(tokens[9], ";");
    cr_assert_str_eq(tokens[10], "f");
    cr_assert_null(tokens[11]);
    free_array(tokens);
}

Test(tokenize_line, background_operator_without_spaces)
{
    char **tokens = tokenize_line("sleep&echo ok");

    cr_assert_not_null(tokens);
    cr_assert_str_eq(tokens[0], "sleep");
    cr_assert_str_eq(tokens[1], "&");
    cr_assert_str_eq(tokens[2], "echo");
    cr_assert_str_eq(tokens[3], "ok");
    cr_assert_null(tokens[4]);
    free_array(tokens);
}

Test(tokenize_line, unterminated_quote_returns_null)
{
    char **tokens = tokenize_line("echo \"abc");

    cr_assert_null(tokens);
}

Test(tokenize_line, trailing_escape_does_not_crash)
{
    char **tokens = tokenize_line("\\");

    cr_assert_not_null(tokens);
    cr_assert_null(tokens[0]);
    free_array(tokens);
}

Test(parse_logic, and_root)
{
    char *tokens[] = {"echo", "a", "&&", "echo", "b", NULL};
    int pos = 0;
    tree_t *tree = parse_logic(tokens, &pos);

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_AND);
    cr_assert_eq(pos, 5);
    free_tree(tree);
}

Test(parse_logic, or_root)
{
    char *tokens[] = {"echo", "a", "||", "echo", "b", NULL};
    int pos = 0;
    tree_t *tree = parse_logic(tokens, &pos);

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_OR);
    cr_assert_eq(pos, 5);
    free_tree(tree);
}

Test(parse_logic, left_null_returns_null)
{
    char *tokens[] = {"&&", "echo", "a", NULL};
    int pos = 0;
    tree_t *tree = parse_logic(tokens, &pos);

    cr_assert_null(tree);
}

Test(parse_logic, missing_right_returns_null)
{
    char *tokens[] = {"echo", "a", "&&", NULL};
    int pos = 0;
    tree_t *tree = parse_logic(tokens, &pos);

    cr_assert_null(tree);
}

Test(parse_logic, chain_and_or)
{
    char *tokens[] = {"echo", "a", "&&", "echo", "b", "||", "echo", "c", NULL};
    int pos = 0;
    tree_t *tree = parse_logic(tokens, &pos);

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_OR);
    cr_assert_not_null(tree->left);
    cr_assert_eq(tree->left->type, TOKEN_AND);
    cr_assert_eq(pos, 8);
    free_tree(tree);
}

Test(parse_logic, get_logic_type_values)
{
    cr_assert_eq(get_logic_type("&&"), TOKEN_AND);
    cr_assert_eq(get_logic_type("||"), TOKEN_OR);
    cr_assert_eq(get_logic_type("xx"), TOKEN_CMD);
}

Test(parse_sequence, background_node_with_right)
{
    char *tokens[] = {"echo", "a", "&", "echo", "b", NULL};
    int pos = 0;
    tree_t *tree = parse_sequence(tokens, &pos);

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_BACKGROUND);
    cr_assert_not_null(tree->left);
    cr_assert_not_null(tree->right);
    cr_assert_eq(pos, 5);
    free_tree(tree);
}

Test(parse_sequence, trailing_background_operator_is_valid)
{
    char *tokens[] = {"echo", "a", "&", NULL};
    int pos = 0;
    tree_t *tree = parse_sequence(tokens, &pos);

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_BACKGROUND);
    cr_assert_not_null(tree->left);
    cr_assert_null(tree->right);
    cr_assert_eq(pos, 3);
    free_tree(tree);
}
