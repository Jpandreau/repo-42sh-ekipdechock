/*
** EPITECH PROJECT, 2026
** exec_tree
** File description:
** exec_tree
*/

#include "my.h"
#include "tree.h"
#include "shell.h"

static int exec_cmd_node_nofork_impl(tree_t *node, char ***env,
    exec_ctx_t *ctx)
{
    if (node == NULL)
        return 0;
    if (ctx != NULL && ctx->shell != NULL && is_shell_builtin(node->args[0]))
        return run_shell_builtin(node->args, ctx->shell);
    return actions_cmd_args_nofork(node->args, env,
        ctx ? ctx->history : NULL, ctx ? ctx->job : NULL);
}

int exec_cmd_node_nofork(tree_t *node, char ***env, exec_ctx_t *ctx)
{
    return exec_cmd_node_nofork_impl(node, env, ctx);
}

int prepare_and_exec_cmd(tree_t *node, char ***env, exec_ctx_t *ctx)
{
    if (node == NULL)
        return 0;
    return exec_cmd_with_redirections(node, env, ctx);
}

static int exec_logic_and(tree_t *node, char ***env, exec_ctx_t *ctx)
{
    int status = exec_tree(node->left, env, ctx);

    if (is_exit_status(status))
        return status;
    if (status == 0)
        return exec_tree(node->right, env, ctx);
    return status;
}

static int exec_logic_or(tree_t *node, char ***env, exec_ctx_t *ctx)
{
    int status = exec_tree(node->left, env, ctx);

    if (is_exit_status(status))
        return status;
    if (status != 0)
        return exec_tree(node->right, env, ctx);
    return status;
}

static int handle_sequence(tree_t *node, char ***env, exec_ctx_t *ctx)
{
    int left = exec_tree(node->left, env, ctx);

    return is_exit_status(left) ?
        left : exec_tree(node->right, env, ctx);
}

static int handle_background(tree_t *node, char ***env, exec_ctx_t *ctx)
{
    int status = exec_background(node->left, env, ctx);

    if (status != 0 || node->right == NULL)
        return status;
    return exec_tree(node->right, env, ctx);
}

int exec_tree(tree_t *node, char ***env, exec_ctx_t *ctx)
{
    if (!node)
        return 0;
    if (prepare_tree_heredocs(node) != 0)
        return 84;
    if (node->type == TOKEN_SEQUENCE)
        return handle_sequence(node, env, ctx);
    if (node->type == TOKEN_BACKGROUND)
        return handle_background(node, env, ctx);
    if (node->type == TOKEN_AND)
        return exec_logic_and(node, env, ctx);
    if (node->type == TOKEN_OR)
        return exec_logic_or(node, env, ctx);
    if (node->type == TOKEN_PIPE)
        return exec_pipe(node, env, ctx);
    if (node->type == TOKEN_SUBSHELL)
        return exec_subshell(node, env, ctx);
    return (node->type == TOKEN_CMD) ?
        prepare_and_exec_cmd(node, env, ctx) : 1;
}
