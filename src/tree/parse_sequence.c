/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** parse_sequence
*/

#include "base.h"
#include "tree.h"

static int is_sequence_token(char *token)
{
    if (token == NULL)
        return 0;
    if (my_strcmp(token, ";") == 0)
        return 1;
    return my_strcmp(token, "&") == 0;
}

static tree_t *attach_sequence_node(tree_t *left, token_t **tokens, int *pos)
{
    tree_t *node = new_node(TOKEN_SEQUENCE);
    tree_t *right = NULL;

    if (node == NULL) {
        free_tree(left);
        return NULL;
    }
    (*pos)++;
    right = parse_logic(tokens, pos);
    if (right == NULL) {
        free_tree(left);
        free(node);
        return NULL;
    }
    node->left = left;
    node->right = right;
    return node;
}

static tree_t *attach_background_node(tree_t *left, token_t **tokens, int *pos)
{
    tree_t *node = new_node(TOKEN_BACKGROUND);

    if (node == NULL) {
        free_tree(left);
        return NULL;
    }
    (*pos)++;
    node->left = left;
    if (tokens[*pos] == NULL)
        return node;
    node->right = parse_logic(tokens, pos);
    if (node->right == NULL) {
        free_tree(left);
        free(node);
        return NULL;
    }
    return node;
}

tree_t *parse_sequence(token_t **tokens, int *pos)
{
    tree_t *left = parse_logic(tokens, pos);

    if (left == NULL)
        return NULL;
    while (tokens[*pos] && is_sequence_token(tokens[*pos]->value)) {
        if (my_strcmp(tokens[*pos]->value, "&") == 0)
            left = attach_background_node(left, tokens, pos);
        else
            left = attach_sequence_node(left, tokens, pos);
        if (left == NULL)
            return NULL;
    }
    return left;
}
