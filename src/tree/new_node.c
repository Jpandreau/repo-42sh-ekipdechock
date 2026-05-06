/*
** EPITECH PROJECT, 2026
** new_node
** File description:
** new_node
*/

#include "base.h"
#include "tree.h"

tree_t *new_node(tree_type_t type)
{
    tree_t *node = malloc(sizeof(tree_t));

    if (!node)
        return (NULL);
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->input = NULL;
    node->output = NULL;
    node->append = 0;
    node->heredoc = 0;
    node->heredoc_fd = -1;
    node->args = NULL;
    node->arg_types = NULL;
    return node;
}
