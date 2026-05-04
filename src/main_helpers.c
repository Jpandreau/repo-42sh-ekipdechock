/*
** EPITECH PROJECT, 2026
** main_helpers
** File description:
** main_helpers
*/

#include "my.h"
#include "base.h"
#include "small_headers.h"
#include "tree.h"

static int close_error(int file)
{
    close(file);
    return 84;
}

static int prepare_pipe_line(char *input_line)
{
    if (input_line[0] == '\n')
        return 1;
    input_line[my_strlen(input_line) - 1] = '\0';
    return 0;
}

int run_line(char *input_line, char ***env, int *exit_code, exec_ctx_t *ctx)
{
    tree_t *tree = NULL;
    int code = 0;

    if (parse_exit_line(input_line, &code) == 1) {
        *exit_code = code;
        return 1;
    }
    tree = get_tree_token(input_line);
    if (tree == NULL)
        return 0;
    *exit_code = exec_tree(tree, env, ctx);
    free_tree(tree);
    if (*exit_code == 84) {
        *exit_code = 0;
        return 1;
    }
    return 0;
}

int open_and_stat(char *filename, struct stat *st)
{
    int file = open(filename, O_RDONLY);

    if (file == -1)
        return -1;
    if (stat(filename, st) == -1)
        return close_error(file);
    return file;
}

int read_content(int file, struct stat *st, char **content)
{
    *content = malloc(sizeof(char) * (st->st_size + 1));
    if (*content == NULL)
        return close_error(file);
    read(file, *content, st->st_size);
    (*content)[st->st_size] = '\0';
    close(file);
    return 0;
}

int handle_pipe_line(char *input_line, char ***env, int *exit_code,
    exec_ctx_t *ctx)
{
    tree_t *tree = NULL;
    int code = 0;

    if (prepare_pipe_line(input_line))
        return 0;
    if (parse_exit_line(input_line, &code) == 1) {
        *exit_code = code;
        return 1;
    }
    tree = get_tree_token(input_line);
    if (tree == NULL)
        return 0;
    *exit_code = exec_tree(tree, env, ctx);
    free_tree(tree);
    return 0;
}
