/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** direct tests for tokenize_line and parse_logic
*/

#include <criterion/criterion.h>
#include "small_headers.h"
#include "tree.h"
#include "base.h"
#include "tokenize_line_helpers.h"

tree_type_t get_logic_type(char *token);

static token_t **make_test_tokens(char **strs)
{
    token_t **toks = NULL;
    int size = 0;
    int i = 0;

    while (strs[size] != NULL)
        size++;
    toks = malloc(sizeof(token_t *) * (size + 1));
    if (toks == NULL)
        return NULL;
    while (i < size) {
        toks[i] = malloc(sizeof(token_t));
        toks[i]->value = my_strdup(strs[i]);
        toks[i]->type = TOKEN_TYPE_WORD;
        i++;
    }
    toks[size] = NULL;
    return toks;
}

Test(tokenize_line, quoted_word_kept_together)
{
    token_t **tokens = tokenize_line("echo 'ab cd'");

    cr_assert_not_null(tokens);
    cr_assert_str_eq(tokens[0]->value, "echo");
    cr_assert_eq(tokens[0]->type, TOKEN_TYPE_WORD);
    cr_assert_str_eq(tokens[1]->value, "ab cd");
    cr_assert_eq(tokens[1]->type, TOKEN_TYPE_SINGLE_QUOTE);
    cr_assert_null(tokens[2]);
    free_tokens(tokens);
}

Test(tokenize_line, escaped_space_kept_in_token)
{
    token_t **tokens = tokenize_line("a\\ b");

    cr_assert_not_null(tokens);
    cr_assert_str_eq(tokens[0]->value, "a b");
    cr_assert_eq(tokens[0]->type, TOKEN_TYPE_WORD);
    cr_assert_null(tokens[1]);
    free_tokens(tokens);
}

Test(tokenize_line, operators_without_spaces)
{
    token_t **tokens = tokenize_line("a&&b||c>>d<<e;f");

    cr_assert_not_null(tokens);
    cr_assert_str_eq(tokens[0]->value, "a");
    cr_assert_str_eq(tokens[1]->value, "&&");
    cr_assert_eq(tokens[1]->type, TOKEN_TYPE_OPERATOR);
    cr_assert_str_eq(tokens[2]->value, "b");
    cr_assert_str_eq(tokens[3]->value, "||");
    cr_assert_str_eq(tokens[4]->value, "c");
    cr_assert_str_eq(tokens[5]->value, ">>");
    cr_assert_str_eq(tokens[6]->value, "d");
    cr_assert_str_eq(tokens[7]->value, "<<");
    cr_assert_str_eq(tokens[8]->value, "e");
    cr_assert_str_eq(tokens[9]->value, ";");
    cr_assert_str_eq(tokens[10]->value, "f");
    cr_assert_null(tokens[11]);
    free_tokens(tokens);
}

Test(tokenize_line, background_operator_without_spaces)
{
    token_t **tokens = tokenize_line("sleep&echo ok");

    cr_assert_not_null(tokens);
    cr_assert_str_eq(tokens[0]->value, "sleep");
    cr_assert_str_eq(tokens[1]->value, "&");
    cr_assert_str_eq(tokens[2]->value, "echo");
    cr_assert_str_eq(tokens[3]->value, "ok");
    cr_assert_null(tokens[4]);
    free_tokens(tokens);
}

Test(tokenize_line, unterminated_quote_returns_null)
{
    token_t **tokens = tokenize_line("echo \"abc");

    cr_assert_null(tokens);
}

Test(tokenize_line, trailing_escape_does_not_crash)
{
    token_t **tokens = tokenize_line("\\");

    cr_assert_not_null(tokens);
    cr_assert_null(tokens[0]);
    free_tokens(tokens);
}

Test(tokenize_line, backtick_token_type)
{
    token_t **tokens = tokenize_line("`date`");

    cr_assert_not_null(tokens);
    cr_assert_str_eq(tokens[0]->value, "date");
    cr_assert_eq(tokens[0]->type, TOKEN_TYPE_BACKTICK);
    cr_assert_null(tokens[1]);
    free_tokens(tokens);
}

Test(tokenize_line, double_quote_token_type)
{
    token_t **tokens = tokenize_line("\"hello world\"");

    cr_assert_not_null(tokens);
    cr_assert_str_eq(tokens[0]->value, "hello world");
    cr_assert_eq(tokens[0]->type, TOKEN_TYPE_DOUBLE_QUOTE);
    cr_assert_null(tokens[1]);
    free_tokens(tokens);
}

Test(tokenize_line, subshell_parens_tokenized)
{
    token_t **tokens = tokenize_line("(echo foo)");

    cr_assert_not_null(tokens);
    cr_assert_str_eq(tokens[0]->value, "(");
    cr_assert_eq(tokens[0]->type, TOKEN_TYPE_SUBSHELL_OPEN);
    cr_assert_str_eq(tokens[1]->value, "echo");
    cr_assert_str_eq(tokens[2]->value, "foo");
    cr_assert_str_eq(tokens[3]->value, ")");
    cr_assert_eq(tokens[3]->type, TOKEN_TYPE_SUBSHELL_CLOSE);
    cr_assert_null(tokens[4]);
    free_tokens(tokens);
}

Test(parse_logic, and_root)
{
    char *strs[] = {"echo", "a", "&&", "echo", "b", NULL};
    token_t **tokens = make_test_tokens(strs);
    int pos = 0;
    tree_t *tree = parse_logic(tokens, &pos);

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_AND);
    cr_assert_eq(pos, 5);
    free_tree(tree);
    free_tokens(tokens);
}

Test(parse_logic, or_root)
{
    char *strs[] = {"echo", "a", "||", "echo", "b", NULL};
    token_t **tokens = make_test_tokens(strs);
    int pos = 0;
    tree_t *tree = parse_logic(tokens, &pos);

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_OR);
    cr_assert_eq(pos, 5);
    free_tree(tree);
    free_tokens(tokens);
}

Test(parse_logic, left_null_returns_null)
{
    char *strs[] = {"&&", "echo", "a", NULL};
    token_t **tokens = make_test_tokens(strs);
    int pos = 0;
    tree_t *tree = parse_logic(tokens, &pos);

    cr_assert_null(tree);
    free_tokens(tokens);
}

Test(parse_logic, missing_right_returns_null)
{
    char *strs[] = {"echo", "a", "&&", NULL};
    token_t **tokens = make_test_tokens(strs);
    int pos = 0;
    tree_t *tree = parse_logic(tokens, &pos);

    cr_assert_null(tree);
    free_tokens(tokens);
}

Test(parse_logic, chain_and_or)
{
    char *strs[] = {"echo", "a", "&&", "echo", "b", "||", "echo", "c", NULL};
    token_t **tokens = make_test_tokens(strs);
    int pos = 0;
    tree_t *tree = parse_logic(tokens, &pos);

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_OR);
    cr_assert_not_null(tree->left);
    cr_assert_eq(tree->left->type, TOKEN_AND);
    cr_assert_eq(pos, 8);
    free_tree(tree);
    free_tokens(tokens);
}

Test(parse_logic, get_logic_type_values)
{
    cr_assert_eq(get_logic_type("&&"), TOKEN_AND);
    cr_assert_eq(get_logic_type("||"), TOKEN_OR);
    cr_assert_eq(get_logic_type("xx"), TOKEN_CMD);
}

Test(parse_sequence, background_node_with_right)
{
    char *strs[] = {"echo", "a", "&", "echo", "b", NULL};
    token_t **tokens = make_test_tokens(strs);
    int pos = 0;
    tree_t *tree = parse_sequence(tokens, &pos);

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_BACKGROUND);
    cr_assert_not_null(tree->left);
    cr_assert_not_null(tree->right);
    cr_assert_eq(pos, 5);
    free_tree(tree);
    free_tokens(tokens);
}

Test(parse_sequence, trailing_background_operator_is_valid)
{
    char *strs[] = {"echo", "a", "&", NULL};
    token_t **tokens = make_test_tokens(strs);
    int pos = 0;
    tree_t *tree = parse_sequence(tokens, &pos);

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_BACKGROUND);
    cr_assert_not_null(tree->left);
    cr_assert_null(tree->right);
    cr_assert_eq(pos, 3);
    free_tree(tree);
    free_tokens(tokens);
}
