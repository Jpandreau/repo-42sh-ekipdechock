/*
** EPITECH PROJECT, 2026
** free_tree
** File description:
** free_tree
*/

#include "tree.h"
#include <stdlib.h>
#include <unistd.h>

void free_tree(tree_t *node)
{
    if (!node)
        return;
    free_tree(node->left);
    free_tree(node->right);
    if (node->args) {
        for (int i = 0; node->args[i]; i++)
            free(node->args[i]);
        free(node->args);
    }
    if (node->heredoc_fd != -1)
        close(node->heredoc_fd);
    if (node->input)
        free(node->input);
    if (node->output)
        free(node->output);
    free(node);
}
