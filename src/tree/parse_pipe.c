/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** parse_pipe
*/

#include "base.h"
#include "tree.h"

tree_t *parse_pipe(token_t **tokens, int *pos)
{
    tree_t *left = parse_primary(tokens, pos);
    tree_t *node = NULL;
    tree_t *right = NULL;

    if (left == NULL)
        return NULL;
    while (tokens[*pos] && my_strcmp(tokens[*pos]->value, "|") == 0) {
        node = new_node(TOKEN_PIPE);
        if (node == NULL)
            return free_tree(left), NULL;
        (*pos)++;
        right = parse_primary(tokens, pos);
        if (right == NULL)
            return free_tree(left), free_tree(node), NULL;
        node->left = left;
        node->right = right;
        left = node;
    }
    return left;
}
