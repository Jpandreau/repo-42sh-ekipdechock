/*
** EPITECH PROJECT, 2026
** exec_tree
** File description:
** exec_tree
*/

#include "my.h"
#include "tree.h"

static int exec_cmd_node(tree_t *node, char ***env, int nofork,
    history_t *history)
{
    if (node == NULL)
        return 0;
    if (nofork)
        return actions_cmd_args_nofork(node->args, env, history);
    return exec_cmd_with_redirections(node, env, history);
}

int exec_cmd_node_nofork(tree_t *node, char ***env, history_t *history)
{
    return exec_cmd_node(node, env, 1, history);
}

int prepare_and_exec_cmd(tree_t *node, char ***env, history_t *history)
{
    return exec_cmd_node(node, env, 0, history);
}

int exec_logic_and(tree_t *node, char ***env, history_t *history)
{
    int status = exec_tree(node->left, env, history);

    if (is_exit_status(status))
        return status;
    if (status == 0)
        return exec_tree(node->right, env, history);
    return status;
}

int exec_logic_or(tree_t *node, char ***env, history_t *history)
{
    int status = exec_tree(node->left, env, history);

    if (is_exit_status(status))
        return status;
    if (status != 0)
        return exec_tree(node->right, env, history);
    return status;
}

static int handle_sequence(tree_t *node, char ***env, history_t *history)
{
    int left = exec_tree(node->left, env, history);

    return is_exit_status(left) ? left : exec_tree(node->right, env, history);
}

int exec_tree(tree_t *node, char ***env, history_t *history)
{
    if (!node)
        return 0;
    if (prepare_tree_heredocs(node) != 0)
        return 84;
    if (node->type == TOKEN_SEQUENCE)
        return handle_sequence(node, env, history);
    if (node->type == TOKEN_AND)
        return exec_logic_and(node, env, history);
    if (node->type == TOKEN_OR)
        return exec_logic_or(node, env, history);
    if (node->type == TOKEN_PIPE)
        return exec_pipe(node, env, history);
    return (node->type == TOKEN_CMD) ?
        prepare_and_exec_cmd(node, env, history) : 1;
}
