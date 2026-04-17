/*
** EPITECH PROJECT, 2026
** exec_pipe
** File description:
** exec_pipe
*/

#include "base.h"
#include "tree.h"
#include "my.h"
#include "buildin.h"

static int count_pipe_nodes(tree_t *node)
{
    if (node == NULL)
        return 0;
    if (node->type != TOKEN_PIPE)
        return 1;
    return count_pipe_nodes(node->left) + count_pipe_nodes(node->right);
}

static int flatten_pipe_nodes(tree_t *node, tree_t **cmds, int idx)
{
    if (node->type != TOKEN_PIPE) {
        cmds[idx] = node;
        return idx + 1;
    }
    idx = flatten_pipe_nodes(node->left, cmds, idx);
    return flatten_pipe_nodes(node->right, cmds, idx);
}

static int run_pipe_child_cmd(pipe_exec_ctx_t *ctx, int idx)
{
    tree_t *cmd = ctx->cmds[idx];
    int status = 0;

    if (cmd->input != NULL || cmd->output != NULL) {
        status = exec_cmd_with_redirections(cmd, ctx->env);
        exit(status);
    }
    if (cmd->args == NULL || cmd->args[0] == NULL)
        exit(0);
    if (buildin(cmd->args[0]))
        exit(run_buildin_args(cmd->args, ctx->env));
    exit(exec_cmd_args_nofork(cmd->args, *ctx->env));
}

static void setup_child_pipes(pipe_exec_ctx_t *ctx, int idx)
{
    if (ctx->prev_read != -1)
        dup2(ctx->prev_read, STDIN_FILENO);
    if (idx < ctx->count - 1)
        dup2(ctx->fd_pipe[1], STDOUT_FILENO);
    if (ctx->prev_read != -1)
        close(ctx->prev_read);
    if (idx < ctx->count - 1) {
        close(ctx->fd_pipe[0]);
        close(ctx->fd_pipe[1]);
    }
}

static int spawn_pipe_child(pipe_exec_ctx_t *ctx, int idx)
{
    if (idx < ctx->count - 1 && pipe(ctx->fd_pipe) == -1)
        return 84;
    ctx->pids[idx] = fork();
    if (ctx->pids[idx] == -1)
        return 84;
    if (ctx->pids[idx] == 0) {
        setup_child_pipes(ctx, idx);
        run_pipe_child_cmd(ctx, idx);
    }
    ctx->spawned++;
    if (ctx->prev_read != -1)
        close(ctx->prev_read);
    if (idx < ctx->count - 1)
        close(ctx->fd_pipe[1]);
    if (idx < ctx->count - 1)
        ctx->prev_read = ctx->fd_pipe[0];
    return 0;
}

static int wait_pipe_children(pipe_exec_ctx_t *ctx)
{
    int status = 0;
    int last = 0;

    for (int i = 0; i < ctx->count; i++) {
        waitpid(ctx->pids[i], &status, 0);
        if (i == ctx->count - 1)
            last = status;
    }
    if (WIFSIGNALED(last))
        return 128 + WTERMSIG(last);
    return WEXITSTATUS(last);
}

static int init_pipe_ctx(pipe_exec_ctx_t *ctx, tree_t *node, char ***env)
{
    ctx->count = count_pipe_nodes(node);
    ctx->cmds = malloc(sizeof(tree_t *) * ctx->count);
    ctx->pids = malloc(sizeof(pid_t) * ctx->count);
    ctx->env = env;
    ctx->spawned = 0;
    ctx->prev_read = -1;
    if (ctx->cmds == NULL || ctx->pids == NULL) {
        free(ctx->cmds);
        free(ctx->pids);
        return 84;
    }
    flatten_pipe_nodes(node, ctx->cmds, 0);
    return 0;
}

static void cleanup_failed_pipe(pipe_exec_ctx_t *ctx)
{
    int status = 0;

    if (ctx->prev_read != -1)
        close(ctx->prev_read);
    for (int i = 0; i < ctx->spawned; i++)
        kill(ctx->pids[i], SIGTERM);
    for (int i = 0; i < ctx->spawned; i++)
        waitpid(ctx->pids[i], &status, 0);
}

int exec_pipe(tree_t *node, char ***env)
{
    pipe_exec_ctx_t ctx;
    int ret = 0;

    if (init_pipe_ctx(&ctx, node, env) != 0)
        return 84;
    for (int i = 0; i < ctx.count; i++) {
        if (spawn_pipe_child(&ctx, i) != 0) {
            cleanup_failed_pipe(&ctx);
            free(ctx.cmds);
            free(ctx.pids);
            return 84;
        }
    }
    if (ctx.prev_read != -1)
        close(ctx.prev_read);
    ret = wait_pipe_children(&ctx);
    free(ctx.cmds);
    free(ctx.pids);
    return ret;
}
