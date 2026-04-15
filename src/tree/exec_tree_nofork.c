/*
** EPITECH PROJECT, 2026
** exec_tree_nofork
** File description:
** exec_tree_nofork
*/

#include "tree.h"

static int exec_logic_and_nofork(tree_t *node, char ***env,
    history_t *history)
{
    int status = exec_tree_nofork(node->left, env, history);

    if (is_exit_status(status))
        return status;
    if (status == 0)
        return exec_tree_nofork(node->right, env, history);
    return status;
}

static int exec_logic_or_nofork(tree_t *node, char ***env,
    history_t *history)
{
    int status = exec_tree_nofork(node->left, env, history);

    if (is_exit_status(status))
        return status;
    if (status != 0)
        return exec_tree_nofork(node->right, env, history);
    return status;
}

int exec_tree_nofork(tree_t *node, char ***env, history_t *history)
{
    if (!node)
        return 0;
    if (prepare_tree_heredocs(node) != 0)
        return 84;
    if (node->type == TOKEN_SEQUENCE) {
        exec_tree_nofork(node->left, env, history);
        return exec_tree_nofork(node->right, env, history);
    }
    if (node->type == TOKEN_AND)
        return exec_logic_and_nofork(node, env, history);
    if (node->type == TOKEN_OR)
        return exec_logic_or_nofork(node, env, history);
    if (node->type == TOKEN_PIPE)
        return exec_pipe(node, env, history);
    if (node->type == TOKEN_CMD)
        return exec_cmd_node_nofork(node, env, history);
    return 1;
}
