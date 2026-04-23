/*
** EPITECH PROJECT, 2026
** exec_tree
** File description:
** exec_tree
*/

#include "my.h"
#include "tree.h"

static int exec_cmd_node(tree_t *node, shell_t *shell, int nofork)
{
    if (node == NULL)
        return 0;
    if (nofork)
        return actions_cmd_args_nofork(node->args, shell);
    return exec_cmd_with_redirections(node, shell);
}

int exec_cmd_node_nofork(tree_t *node, shell_t *shell)
{
    return exec_cmd_node(node, shell, 1);
}

int prepare_and_exec_cmd(tree_t *node, shell_t *shell)
{
    return exec_cmd_node(node, shell, 0);
}

int exec_logic_and(tree_t *node, shell_t *shell)
{
    int status = exec_tree(node->left, shell);

    if (is_exit_status(status))
        return status;
    if (status == 0)
        return exec_tree(node->right, shell);
    return status;
}

int exec_logic_or(tree_t *node, shell_t *shell)
{
    int status = exec_tree(node->left, shell);

    if (is_exit_status(status))
        return status;
    if (status != 0)
        return exec_tree(node->right, shell);
    return status;
}

int exec_tree(tree_t *node, shell_t *shell)
{
    if (!node)
        return 0;
    if (prepare_tree_heredocs(node) != 0)
        return 84;
    if (node->type == TOKEN_SEQUENCE) {
        exec_tree(node->left, shell);
        return exec_tree(node->right, shell);
    }
    if (node->type == TOKEN_AND)
        return exec_logic_and(node, shell);
    if (node->type == TOKEN_OR)
        return exec_logic_or(node, shell);
    if (node->type == TOKEN_PIPE)
        return exec_pipe(node, shell);
    if (node->type == TOKEN_CMD)
        return prepare_and_exec_cmd(node, shell);
    return 1;
}
