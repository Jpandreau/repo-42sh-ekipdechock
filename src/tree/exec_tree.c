/*
** EPITECH PROJECT, 2026
** exec_tree
** File description:
** exec_tree
*/

#include "my.h"
#include "tree.h"

static int exec_cmd_node(tree_t *node, char ***env, int nofork)
{
    if (node == NULL)
        return 0;
    if (nofork)
        return actions_cmd_args_nofork(node->args, env);
    return exec_cmd_with_redirections(node, env);
}

int exec_cmd_node_nofork(tree_t *node, char ***env)
{
    return exec_cmd_node(node, env, 1);
}

int prepare_and_exec_cmd(tree_t *node, char ***env)
{
    return exec_cmd_node(node, env, 0);
}

int exec_logic_and(tree_t *node, char ***env)
{
    int status = exec_tree(node->left, env);

    if (is_exit_status(status))
        return status;
    if (status == 0)
        return exec_tree(node->right, env);
    return status;
}

int exec_logic_or(tree_t *node, char ***env)
{
    int status = exec_tree(node->left, env);

    if (is_exit_status(status))
        return status;
    if (status != 0)
        return exec_tree(node->right, env);
    return status;
}

static int exec_sequence(tree_t *node, char ***env)
{
    int left_status = exec_tree(node->left, env);

    if (is_exit_status(left_status))
        return left_status;
    return exec_tree(node->right, env);
}

int exec_tree(tree_t *node, char ***env)
{
    if (!node)
        return 0;
    if (prepare_tree_heredocs(node) != 0)
        return 84;
    if (node->type == TOKEN_SEQUENCE)
        return exec_sequence(node, env);
    if (node->type == TOKEN_AND)
        return exec_logic_and(node, env);
    if (node->type == TOKEN_OR)
        return exec_logic_or(node, env);
    if (node->type == TOKEN_PIPE)
        return exec_pipe(node, env);
    if (node->type == TOKEN_CMD)
        return prepare_and_exec_cmd(node, env);
    return 1;
}
