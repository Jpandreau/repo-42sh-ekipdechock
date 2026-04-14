/*
** EPITECH PROJECT, 2026
** parse_sequence
** File description:
** parse_sequence
*/

#include "base.h"
#include "tree.h"

static tree_t *attach_sequence_node(tree_t *left, char **tokens, int *pos)
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

tree_t *parse_sequence(char **tokens, int *pos)
{
    tree_t *left = parse_logic(tokens, pos);

    if (left == NULL)
        return NULL;
    while (tokens[*pos] && my_strcmp(tokens[*pos], ";") == 0) {
        left = attach_sequence_node(left, tokens, pos);
        if (left == NULL)
            return NULL;
    }
    return left;
}
