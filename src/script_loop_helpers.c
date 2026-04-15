/*
** EPITECH PROJECT, 2026
** script_loop_helpers
** File description:
** script_loop_helpers
*/

#include "base.h"
#include "tree.h"
#include "small_headers.h"

int init_exec(char **line, char ***env, history_t *history)
{
    tree_t *tree = get_tree_token(*line);
    int status = 0;

    free(*line);
    *line = NULL;
    if (tree) {
        status = exec_tree(tree, env, history);
        free_tree(tree);
    }
    return status;
}

int handle_line(char **line, char ***env, history_t *history)
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
    status = init_exec(line, env, history);
    return status;
}
