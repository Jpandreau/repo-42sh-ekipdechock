/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** parse_logic
*/

#include "base.h"
#include "tree.h"

tree_type_t get_logic_type(char *token)
{
    if (my_strncmp(token, "&&", 2) == 0)
        return TOKEN_AND;
    if (my_strncmp(token, "||", 2) == 0)
        return TOKEN_OR;
    return TOKEN_CMD;
}

static int is_logic_token(char *token)
{
    if (token == NULL)
        return 0;
    if (my_strcmp(token, "&&") == 0)
        return 1;
    return my_strcmp(token, "||") == 0;
}

static tree_t *attach_logic_node(tree_t *left, token_t **tokens, int *pos)
{
    tree_t *node = new_node(get_logic_type(tokens[*pos]->value));
    tree_t *right = NULL;

    if (node == NULL) {
        free_tree(left);
        return NULL;
    }
    (*pos)++;
    right = parse_pipe(tokens, pos);
    if (right == NULL) {
        free_tree(left);
        free(node);
        return NULL;
    }
    node->left = left;
    node->right = right;
    return node;
}

tree_t *parse_logic(token_t **tokens, int *pos)
{
    tree_t *left = parse_pipe(tokens, pos);

    if (left == NULL)
        return NULL;
    while (tokens[*pos] && is_logic_token(tokens[*pos]->value)) {
        left = attach_logic_node(left, tokens, pos);
        if (left == NULL)
            return NULL;
    }
    return left;
}
