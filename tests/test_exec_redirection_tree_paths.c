/*
** EPITECH PROJECT, 2026
** test_exec_redirection_tree_paths
** File description:
** extra coverage for exec_redirection and exec_tree
*/

#include <criterion/criterion.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "base.h"

static tree_t *make_cmd_node(char *cmd)
{
    tree_t *node = new_node(TOKEN_CMD);

    node->args = malloc(sizeof(char *) * 2);
    if (node->args == NULL)
        return node;
    node->args[0] = my_strdup(cmd);
    node->args[1] = NULL;
    return node;
}

static char *read_file_content(char *path)
{
    FILE *file = fopen(path, "r");
    long size = 0;
    char *content = NULL;

    if (file == NULL)
        return NULL;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    content = malloc((size_t)size + 1);
    if (content == NULL)
        return fclose(file), NULL;
    fread(content, 1, (size_t)size, file);
    content[size] = '\0';
    fclose(file);
    return content;
}

Test(exec_redirection, input_missing_returns_1)
{
    char *env_data[] = {"PATH=/bin:/usr/bin", NULL};
    char **env = env_data;
    tree_t *node = make_cmd_node("cat");

    cr_assert_not_null(node);
    node->input = my_strdup("/no/such/input/file");
    cr_assert_eq(exec_cmd_with_redirections(node, &env), 1);
    free_tree(node);
}

Test(exec_redirection, output_open_error_returns_1)
{
    char *env_data[] = {"PATH=/bin:/usr/bin", NULL};
    char **env = env_data;
    tree_t *node = make_cmd_node("echo");

    cr_assert_not_null(node);
    node->output = my_strdup("/no/such/dir/out.txt");
    cr_assert_eq(exec_cmd_with_redirections(node, &env), 1);
    free_tree(node);
}

Test(exec_redirection, heredoc_fd_prepared_path)
{
    char *env_data[] = {"PATH=/bin:/usr/bin", NULL};
    char **env = env_data;
    tree_t *node = make_cmd_node("cat");
    int fd_pipe[2] = {0};
    char *out = "/tmp/ms_exec_redir_hd_out1";
    char *content = NULL;

    unlink(out);
    node->output = my_strdup(out);
    node->heredoc = 1;
    node->input = my_strdup("EOF");
    cr_assert_eq(pipe(fd_pipe), 0);
    write(fd_pipe[1], "lineA\n", 6);
    close(fd_pipe[1]);
    node->heredoc_fd = fd_pipe[0];
    cr_assert_eq(exec_cmd_with_redirections(node, &env), 0);
    cr_assert_eq(node->heredoc_fd, -1);
    content = read_file_content(out);
    cr_assert_not_null(content);
    cr_assert_str_eq(content, "lineA\n");
    free(content);
    unlink(out);
    free_tree(node);
}

Test(exec_redirection, heredoc_dynamic_path)
{
    char *env_data[] = {"PATH=/bin:/usr/bin", NULL};
    char **env = env_data;
    tree_t *node = make_cmd_node("cat");
    int saved_stdin = dup(STDIN_FILENO);
    int fd_pipe[2] = {0};
    char *out = "/tmp/ms_exec_redir_hd_out2";
    char *content = NULL;

    unlink(out);
    node->output = my_strdup(out);
    node->heredoc = 1;
    node->input = my_strdup("EOF");
    cr_assert_eq(pipe(fd_pipe), 0);
    write(fd_pipe[1], "lineB\nEOF\n", 10);
    close(fd_pipe[1]);
    dup2(fd_pipe[0], STDIN_FILENO);
    close(fd_pipe[0]);
    cr_assert_eq(exec_cmd_with_redirections(node, &env), 0);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    content = read_file_content(out);
    cr_assert_not_null(content);
    cr_assert_str_eq(content, "lineB\n");
    free(content);
    unlink(out);
    free_tree(node);
}

Test(exec_tree_paths, cmd_and_unknown)
{
    char *env_data[] = {"PATH=/bin:/usr/bin", NULL};
    char **env = env_data;
    tree_t *cmd = make_cmd_node("env");
    tree_t *unknown = new_node(TOKEN_EOF);

    cr_assert_eq(exec_tree(cmd, &env), 0);
    cr_assert_eq(exec_tree(unknown, &env), 1);
    free_tree(cmd);
    free_tree(unknown);
}

Test(exec_tree_paths, and_or_sequence_paths)
{
    char *env_data[] = {"PATH=/bin:/usr/bin", NULL};
    char **env = env_data;
    tree_t *and_node = new_node(TOKEN_AND);
    tree_t *or_node = new_node(TOKEN_OR);
    tree_t *seq_node = new_node(TOKEN_SEQUENCE);

    and_node->left = new_node(TOKEN_EOF);
    and_node->right = make_cmd_node("env");
    cr_assert_eq(exec_tree(and_node, &env), 1);
    or_node->left = new_node(TOKEN_EOF);
    or_node->right = make_cmd_node("env");
    cr_assert_eq(exec_tree(or_node, &env), 0);
    seq_node->left = new_node(TOKEN_EOF);
    seq_node->right = new_node(TOKEN_EOF);
    cr_assert_eq(exec_tree(seq_node, &env), 1);
    free_tree(and_node);
    free_tree(or_node);
    free_tree(seq_node);
}
