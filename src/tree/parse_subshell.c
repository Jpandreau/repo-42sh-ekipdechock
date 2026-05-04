/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** parse subshell (cmd) expressions
*/

#include "base.h"
#include "tree.h"

tree_t *parse_subshell(token_t **tokens, int *pos)
{
    tree_t *node = new_node(TOKEN_SUBSHELL);
    tree_t *inner = NULL;

    if (node == NULL)
        return NULL;
    (*pos)++;
    inner = parse_sequence(tokens, pos);
    if (inner == NULL || tokens[*pos] == NULL
        || my_strcmp(tokens[*pos]->value, ")") != 0) {
        free_tree(node);
        free_tree(inner);
        return NULL;
    }
    (*pos)++;
    node->left = inner;
    return node;
}

tree_t *parse_primary(token_t **tokens, int *pos)
{
    if (tokens[*pos] == NULL)
        return NULL;
    if (tokens[*pos]->type == TOKEN_TYPE_SUBSHELL_OPEN)
        return parse_subshell(tokens, pos);
    return parse_command(tokens, pos);
}
