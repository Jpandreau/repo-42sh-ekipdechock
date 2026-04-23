/*
** EPITECH PROJECT, 2026
** exec_tree_nofork
** File description:
** exec_tree_nofork
*/

#include "tree.h"

static int exec_logic_and_nofork(tree_t *node, shell_t *shell)
{
    int status = exec_tree_nofork(node->left, shell);

    if (is_exit_status(status))
        return status;
    if (status == 0)
        return exec_tree_nofork(node->right, shell);
    return status;
}

static int exec_logic_or_nofork(tree_t *node, shell_t *shell)
{
    int status = exec_tree_nofork(node->left, shell);

    if (is_exit_status(status))
        return status;
    if (status != 0)
        return exec_tree_nofork(node->right, shell);
    return status;
}

int exec_tree_nofork(tree_t *node, shell_t *shell)
{
    if (!node)
        return 0;
    if (prepare_tree_heredocs(node) != 0)
        return 84;
    if (node->type == TOKEN_SEQUENCE) {
        exec_tree_nofork(node->left, shell);
        return exec_tree_nofork(node->right, shell);
    }
    if (node->type == TOKEN_AND)
        return exec_logic_and_nofork(node, shell);
    if (node->type == TOKEN_OR)
        return exec_logic_or_nofork(node, shell);
    if (node->type == TOKEN_PIPE)
        return exec_pipe(node, shell);
    if (node->type == TOKEN_CMD)
        return exec_cmd_node_nofork(node, shell);
    return 1;
}
