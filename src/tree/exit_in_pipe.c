/*
** EPITECH PROJECT, 2026
** exit_in_pipe
** File description:
** exit_in_pipe
*/

#include "base.h"
#include "tree.h"
#include "small_headers.h"

static int is_exit_cmd_node(tree_t *node)
{
    if (node == NULL || node->type != TOKEN_CMD)
        return 0;
    if (node->args == NULL || node->args[0] == NULL)
        return 0;
    return my_strcmp(node->args[0], "exit") == 0;
}

static int exit_code_from_args_invalid(char **args, int *valid)
{
    int ok = 0;
    int size = 0;
    int code = 0;

    if (args == NULL || args[0] == NULL) {
        *valid = 0;
        return 0;
    }
    size = array_size(args);
    if (size == 1) {
        *valid = 1;
        return 0;
    }
    if (size > 2) {
        *valid = -1;
        return 0;
    }
    code = parse_exit_code_arg(args[1], &ok);
    *valid = ok ? 1 : -1;
    return code;
}

static int handle_in_pipe(tree_t *node, int *code, int *invalid)
{
    int valid = 0;
    int tmp = 0;

    if (!is_exit_cmd_node(node))
        return 0;
    tmp = exit_code_from_args_invalid(node->args, &valid);
    if (valid == 1) {
        *code = tmp;
        return 1;
    }
    if (valid == -1)
        *invalid = 1;
    return 0;
}

static int find_exit(tree_t *node, int in_pipe, int *code, int *invalid)
{
    if (node == NULL)
        return 0;
    if (node->type == TOKEN_PIPE) {
        if (find_exit(node->left, 1, code, invalid))
            return 1;
        return find_exit(node->right, 1, code, invalid);
    }
    if (in_pipe && handle_in_pipe(node, code, invalid))
        return 1;
    if (find_exit(node->left, in_pipe, code, invalid))
        return 1;
    return find_exit(node->right, in_pipe, code, invalid);
}

int find_exit_in_pipe(tree_t *node, int *code, int *invalid)
{
    if (invalid != NULL)
        *invalid = 0;
    if (code != NULL)
        *code = 0;
    return find_exit(node, 0, code, invalid);
}
