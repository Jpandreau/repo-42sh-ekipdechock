/*
** EPITECH PROJECT, 2026
** parse_pipe
** File description:
** parse_pipe
*/

#include "base.h"
#include "tree.h"

tree_t *parse_pipe(char **tokens, int *pos)
{
    tree_t *left = parse_command(tokens, pos);
    tree_t *node = NULL;
    tree_t *right = NULL;

    if (left == NULL)
        return NULL;
    while (tokens[*pos] && my_strcmp(tokens[*pos], "|") == 0) {
        node = new_node(TOKEN_PIPE);
        if (node == NULL)
            return free_tree(left), NULL;
        (*pos)++;
        right = parse_command(tokens, pos);
        if (right == NULL)
            return free_tree(left), free_tree(node), NULL;
        node->left = left;
        node->right = right;
        left = node;
    }
    return left;
}
