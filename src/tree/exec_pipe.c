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
#include "shell.h"

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

static void run_pipe_cmd_exec(pipe_exec_ctx_t *pctx, tree_t *cmd)
{
    int status = 0;

    if (pctx->ctx != NULL && pctx->ctx->shell != NULL
        && is_shell_builtin(cmd->args[0]))
        exit(run_shell_builtin(cmd->args, pctx->ctx->shell));
    if (buildin(cmd->args[0])) {
        status = run_buildin_args(cmd->args, pctx->env,
            pctx->ctx ? pctx->ctx->history : NULL,
            pctx->ctx ? pctx->ctx->job : NULL);
        exit(status);
    }
    exit(exec_cmd_args_nofork(cmd->args, *pctx->env));
}

static int run_pipe_child_cmd(pipe_exec_ctx_t *pctx, int idx)
{
    tree_t *cmd = pctx->cmds[idx];
    int status = 0;

    if (cmd->type == TOKEN_SUBSHELL)
        exit(exec_tree(cmd, pctx->env, pctx->ctx));
    if (cmd->input != NULL || cmd->output != NULL) {
        status = exec_cmd_with_redirections(cmd, pctx->env, pctx->ctx);
        exit(status);
    }
    if (cmd->args == NULL || cmd->args[0] == NULL)
        exit(0);
    run_pipe_cmd_exec(pctx, cmd);
    return 0;
}

static void setup_child_pipes(pipe_exec_ctx_t *pctx, int idx)
{
    if (pctx->prev_read != -1)
        dup2(pctx->prev_read, STDIN_FILENO);
    if (idx < pctx->count - 1)
        dup2(pctx->fd_pipe[1], STDOUT_FILENO);
    if (pctx->prev_read != -1)
        close(pctx->prev_read);
    if (idx < pctx->count - 1) {
        close(pctx->fd_pipe[0]);
        close(pctx->fd_pipe[1]);
    }
}

static int spawn_pipe_child(pipe_exec_ctx_t *pctx, int idx)
{
    if (idx < pctx->count - 1 && pipe(pctx->fd_pipe) == -1)
        return 84;
    pctx->pids[idx] = fork();
    if (pctx->pids[idx] == -1)
        return 84;
    if (pctx->pids[idx] == 0) {
        setup_child_pipes(pctx, idx);
        run_pipe_child_cmd(pctx, idx);
    }
    pctx->spawned++;
    if (pctx->prev_read != -1)
        close(pctx->prev_read);
    if (idx < pctx->count - 1)
        close(pctx->fd_pipe[1]);
    if (idx < pctx->count - 1)
        pctx->prev_read = pctx->fd_pipe[0];
    return 0;
}

static int wait_pipe_children(pipe_exec_ctx_t *pctx)
{
    int status = 0;
    int last = 0;

    for (int i = 0; i < pctx->count; i++) {
        waitpid(pctx->pids[i], &status, 0);
        if (i == pctx->count - 1)
            last = status;
    }
    if (WIFSIGNALED(last))
        return 1;
    return WEXITSTATUS(last);
}

static int init_pipe_ctx(pipe_exec_ctx_t *pctx, tree_t *node,
    char ***env, exec_ctx_t *ctx)
{
    pctx->count = count_pipe_nodes(node);
    pctx->cmds = malloc(sizeof(tree_t *) * pctx->count);
    pctx->pids = malloc(sizeof(pid_t) * pctx->count);
    pctx->env = env;
    pctx->ctx = ctx;
    pctx->spawned = 0;
    pctx->prev_read = -1;
    if (pctx->cmds == NULL || pctx->pids == NULL)
        return 84;
    flatten_pipe_nodes(node, pctx->cmds, 0);
    return 0;
}

static void cleanup_failed_pipe(pipe_exec_ctx_t *pctx)
{
    int status = 0;

    if (pctx->prev_read != -1)
        close(pctx->prev_read);
    for (int i = 0; i < pctx->spawned; i++)
        kill(pctx->pids[i], SIGTERM);
    for (int i = 0; i < pctx->spawned; i++)
        waitpid(pctx->pids[i], &status, 0);
}

int exec_pipe(tree_t *node, char ***env, exec_ctx_t *ctx)
{
    pipe_exec_ctx_t pctx;
    int ret = 0;

    if (init_pipe_ctx(&pctx, node, env, ctx) != 0)
        return 84;
    for (int i = 0; i < pctx.count; i++) {
        if (spawn_pipe_child(&pctx, i) != 0) {
            cleanup_failed_pipe(&pctx);
            free(pctx.cmds);
            free(pctx.pids);
            return 84;
        }
    }
    if (pctx.prev_read != -1)
        close(pctx.prev_read);
    ret = wait_pipe_children(&pctx);
    free(pctx.cmds);
    free(pctx.pids);
    return ret;
}
