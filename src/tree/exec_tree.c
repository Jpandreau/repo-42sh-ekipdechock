/*
** EPITECH PROJECT, 2026
** exec_tree
** File description:
** exec_tree
*/

#include "my.h"
#include "tree.h"

static int exec_cmd_node_nofork_impl(tree_t *node, char ***env,
    history_t *history, job_state_t *job)
{
    if (node == NULL)
        return 0;
    return actions_cmd_args_nofork(node->args, env, history, job);
}

static int exec_cmd_node_fork_impl(tree_t *node, char ***env,
    history_t *history, job_state_t *job)
{
    if (node == NULL)
        return 0;
    return exec_cmd_with_redirections(node, env, history, job);
}

int exec_cmd_node_nofork(tree_t *node, char ***env, history_t *history,
    job_state_t *job)
{
    return exec_cmd_node_nofork_impl(node, env, history, job);
}

int prepare_and_exec_cmd(tree_t *node, char ***env, history_t *history,
    job_state_t *job)
{
    return exec_cmd_node_fork_impl(node, env, history, job);
}

int exec_logic_and(tree_t *node, char ***env, history_t *history,
    job_state_t *job)
{
    int status = exec_tree(node->left, env, history, job);

    if (is_exit_status(status))
        return status;
    if (status == 0)
        return exec_tree(node->right, env, history, job);
    return status;
}

int exec_logic_or(tree_t *node, char ***env, history_t *history,
    job_state_t *job)
{
    int status = exec_tree(node->left, env, history, job);

    if (is_exit_status(status))
        return status;
    if (status != 0)
        return exec_tree(node->right, env, history, job);
    return status;
}

static int handle_sequence(tree_t *node, char ***env, history_t *history,
    job_state_t *job)
{
    int left = exec_tree(node->left, env, history, job);

    return is_exit_status(left) ?
        left : exec_tree(node->right, env, history, job);
}

static int handle_background(tree_t *node, char ***env, history_t *history,
    job_state_t *job)
{
    int status = exec_background(node->left, env, history, job);

    if (status != 0 || node->right == NULL)
        return status;
    return exec_tree(node->right, env, history, job);
}

int exec_tree(tree_t *node, char ***env, history_t *history, job_state_t *job)
{
    if (!node)
        return 0;
    if (prepare_tree_heredocs(node) != 0)
        return 84;
    if (node->type == TOKEN_SEQUENCE)
        return handle_sequence(node, env, history, job);
    if (node->type == TOKEN_BACKGROUND)
        return handle_background(node, env, history, job);
    if (node->type == TOKEN_AND)
        return exec_logic_and(node, env, history, job);
    if (node->type == TOKEN_OR)
        return exec_logic_or(node, env, history, job);
    if (node->type == TOKEN_PIPE)
        return exec_pipe(node, env, history, job);
    if (node->type == TOKEN_SUBSHELL)
        return exec_subshell(node, env, history, job);
    return (node->type == TOKEN_CMD) ?
        prepare_and_exec_cmd(node, env, history, job) : 1;
}
