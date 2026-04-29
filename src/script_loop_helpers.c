/*
** EPITECH PROJECT, 2026
** script_loop_helpers
** File description:
** script_loop_helpers
*/

#include "base.h"
#include "tree.h"
#include "small_headers.h"
#include "shell.h"

static int exec_shell_builtin(tree_t *tree, shell_t *shell)
{
    int status = 0;

    if (!tree || tree->type != TOKEN_CMD || !tree->args)
        return -1;
    if (!shell || !is_shell_builtin(tree->args[0]))
        return -1;
    status = run_shell_builtin(tree->args, shell);
    free_tree(tree);
    return status;
}

static void apply_expansion(char **line, char **aliases, char **locals,
    char **env)
{
    char *expanded = expand_alias_in_line(*line, aliases);

    if (expanded) {
        free(*line);
        *line = expanded;
    }
    expanded = expand_vars_in_line(*line, locals, env);
    if (expanded) {
        free(*line);
        *line = expanded;
    }
}

int init_exec(char **line, char ***env, exec_ctx_t *ctx)
{
    tree_t *tree = NULL;
    int status = 0;
    int sb_status = 0;

    if (ctx->shell)
        apply_expansion(line, ctx->shell->aliases, ctx->shell->locals, *env);
    tree = get_tree_token(*line);
    free(*line);
    *line = NULL;
    sb_status = exec_shell_builtin(tree, ctx->shell);
    if (sb_status != -1)
        return sb_status;
    if (tree) {
        status = exec_tree(tree, env, ctx->history, ctx->job);
        free_tree(tree);
    }
    return status;
}

int handle_line(char **line, char ***env, exec_ctx_t *ctx)
{
    int code = 0;
    int status = parse_exit_line(*line, &code);

    if (status == 1) {
        free(*line);
        *line = NULL;
        return make_exit_status(code);
    }
    if (status == 84) {
        free(*line);
        *line = NULL;
        return 0;
    }
    status = init_exec(line, env, ctx);
    return status;
}
