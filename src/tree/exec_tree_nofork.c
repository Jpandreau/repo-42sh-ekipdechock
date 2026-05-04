/*
** EPITECH PROJECT, 2026
** exec_tree_nofork
** File description:
** exec_tree_nofork
*/

#include "tree.h"

static int exec_logic_and_nofork(tree_t *node, char ***env, exec_ctx_t *ctx)
{
    int status = exec_tree_nofork(node->left, env, ctx);

    if (is_exit_status(status))
        return status;
    if (status == 0)
        return exec_tree_nofork(node->right, env, ctx);
    return status;
}

static int exec_logic_or_nofork(tree_t *node, char ***env, exec_ctx_t *ctx)
{
    int status = exec_tree_nofork(node->left, env, ctx);

    if (is_exit_status(status))
        return status;
    if (status != 0)
        return exec_tree_nofork(node->right, env, ctx);
    return status;
}

static int handle_sequence_nofork(tree_t *node, char ***env, exec_ctx_t *ctx)
{
    int left = exec_tree_nofork(node->left, env, ctx);

    return is_exit_status(left) ?
        left : exec_tree_nofork(node->right, env, ctx);
}

static int handle_background_nofork(tree_t *node, char ***env,
    exec_ctx_t *ctx)
{
    int left = exec_tree_nofork(node->left, env, ctx);

    if (is_exit_status(left))
        return left;
    if (node->right == NULL)
        return left;
    return exec_tree_nofork(node->right, env, ctx);
}

int exec_tree_nofork(tree_t *node, char ***env, exec_ctx_t *ctx)
{
    if (!node)
        return 0;
    if (prepare_tree_heredocs(node) != 0)
        return 84;
    if (node->type == TOKEN_SEQUENCE)
        return handle_sequence_nofork(node, env, ctx);
    if (node->type == TOKEN_BACKGROUND)
        return handle_background_nofork(node, env, ctx);
    if (node->type == TOKEN_AND)
        return exec_logic_and_nofork(node, env, ctx);
    if (node->type == TOKEN_OR)
        return exec_logic_or_nofork(node, env, ctx);
    if (node->type == TOKEN_PIPE)
        return exec_pipe(node, env, ctx);
    return (node->type == TOKEN_CMD) ?
        exec_cmd_node_nofork(node, env, ctx) : 1;
}
