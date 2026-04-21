/*
** EPITECH PROJECT, 2026
** exec_tree_nofork
** File description:
** exec_tree_nofork
*/

#include "tree.h"

static int exec_logic_and_nofork(tree_t *node, char ***env,
    history_t *history, job_state_t *job)
{
    int status = exec_tree_nofork(node->left, env, history, job);

    if (is_exit_status(status))
        return status;
    if (status == 0)
        return exec_tree_nofork(node->right, env, history, job);
    return status;
}

static int exec_logic_or_nofork(tree_t *node, char ***env,
    history_t *history, job_state_t *job)
{
    int status = exec_tree_nofork(node->left, env, history, job);

    if (is_exit_status(status))
        return status;
    if (status != 0)
        return exec_tree_nofork(node->right, env, history, job);
    return status;
}

static int handle_sequence_nofork(tree_t *node, char ***env,
    history_t *history, job_state_t *job)
{
    int left = exec_tree_nofork(node->left, env, history, job);

    return is_exit_status(left) ?
        left : exec_tree_nofork(node->right, env, history, job);
}

static int handle_background_nofork(tree_t *node, char ***env,
    history_t *history, job_state_t *job)
{
    int left = exec_tree_nofork(node->left, env, history, job);

    if (is_exit_status(left))
        return left;
    if (node->right == NULL)
        return left;
    return exec_tree_nofork(node->right, env, history, job);
}

int exec_tree_nofork(tree_t *node, char ***env, history_t *history,
    job_state_t *job)
{
    if (!node)
        return 0;
    if (prepare_tree_heredocs(node) != 0)
        return 84;
    if (node->type == TOKEN_SEQUENCE)
        return handle_sequence_nofork(node, env, history, job);
    if (node->type == TOKEN_BACKGROUND)
        return handle_background_nofork(node, env, history, job);
    if (node->type == TOKEN_AND)
        return exec_logic_and_nofork(node, env, history, job);
    if (node->type == TOKEN_OR)
        return exec_logic_or_nofork(node, env, history, job);
    if (node->type == TOKEN_PIPE)
        return exec_pipe(node, env, history, job);
    return (node->type == TOKEN_CMD) ?
        exec_cmd_node_nofork(node, env, history, job) : 1;
}
