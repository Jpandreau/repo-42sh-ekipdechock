/*
** EPITECH PROJECT, 2026
** test_tree_operators
** File description:
** Tests parsing/execution for shell operators
*/

#include <criterion/criterion.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"

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

Test(tree_parse, pipe_root)
{
    tree_t *tree = get_tree_token("echo abc | wc -c");

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_PIPE);
    cr_assert_not_null(tree->left);
    cr_assert_not_null(tree->right);
    free_tree(tree);
}

Test(tree_parse, sequence_root)
{
    tree_t *tree = get_tree_token("echo a ; echo b");

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_SEQUENCE);
    cr_assert_not_null(tree->left);
    cr_assert_not_null(tree->right);
    free_tree(tree);
}

Test(tree_parse, redirections_and_append)
{
    tree_t *tree = get_tree_token("cat < in >> out");

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_CMD);
    cr_assert_str_eq(tree->input, "in");
    cr_assert_str_eq(tree->output, "out");
    cr_assert_eq(tree->append, 1);
    free_tree(tree);
}

Test(tree_parse, heredoc_flag)
{
    tree_t *tree = get_tree_token("cat << EOF");

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_CMD);
    cr_assert_str_eq(tree->input, "EOF");
    cr_assert_eq(tree->heredoc, 1);
    free_tree(tree);
}

Test(tree_parse, sequence_has_lower_priority_than_pipe)
{
    tree_t *tree = get_tree_token("echo a | cat ; echo b");

    cr_assert_not_null(tree);
    cr_assert_eq(tree->type, TOKEN_SEQUENCE);
    cr_assert_not_null(tree->left);
    cr_assert_eq(tree->left->type, TOKEN_PIPE);
    free_tree(tree);
}

Test(tree_parse, invalid_trailing_pipe_returns_null)
{
    tree_t *tree = get_tree_token("echo a |");

    cr_assert_null(tree);
}

Test(tree_parse, invalid_lonely_sequence_returns_null)
{
    tree_t *tree = get_tree_token(";");

    cr_assert_null(tree);
}

Test(tree_exec, sequence_with_redirections)
{
    char *env[] = {"PATH=/bin:/usr/bin", NULL};
    char **env_ptr = env;
    char *out = "/tmp/ms_tree_seq_out";
    tree_t *tree = get_tree_token("echo hello > /tmp/ms_tree_seq_in ; cat < /tmp/ms_tree_seq_in > /tmp/ms_tree_seq_out");
    char *content = NULL;

    unlink("/tmp/ms_tree_seq_in");
    unlink(out);
    cr_assert_not_null(tree);
    cr_assert_eq(exec_tree(tree, &env_ptr), 0);
    free_tree(tree);
    content = read_file_content(out);
    cr_assert_not_null(content);
    cr_assert_str_eq(content, "hello\n");
    free(content);
    unlink("/tmp/ms_tree_seq_in");
    unlink(out);
}

Test(tree_exec, pipe_to_file)
{
    char *env[] = {"PATH=/bin:/usr/bin", NULL};
    char **env_ptr = env;
    char *out = "/tmp/ms_tree_pipe_out";
    tree_t *tree = get_tree_token("echo abc | wc -c > /tmp/ms_tree_pipe_out");
    char *content = NULL;

    unlink(out);
    cr_assert_not_null(tree);
    cr_assert_eq(exec_tree(tree, &env_ptr), 0);
    free_tree(tree);
    content = read_file_content(out);
    cr_assert_not_null(content);
    cr_assert_not_null(strstr(content, "4"));
    free(content);
    unlink(out);
}

Test(tree_exec, heredoc_to_file)
{
    char *env[] = {"PATH=/bin:/usr/bin", NULL};
    char **env_ptr = env;
    int saved_stdin = dup(STDIN_FILENO);
    int input_fd = 0;
    char *out = "/tmp/ms_tree_hd_out";
    tree_t *tree = NULL;
    char *content = NULL;

    unlink("/tmp/ms_tree_hd_input");
    unlink(out);
    input_fd = open("/tmp/ms_tree_hd_input", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(input_fd, "line1\nEOF\n", 10);
    close(input_fd);
    input_fd = open("/tmp/ms_tree_hd_input", O_RDONLY);
    dup2(input_fd, STDIN_FILENO);
    close(input_fd);
    tree = get_tree_token("cat << EOF > /tmp/ms_tree_hd_out");
    cr_assert_not_null(tree);
    cr_assert_eq(exec_tree(tree, &env_ptr), 0);
    free_tree(tree);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    content = read_file_content(out);
    cr_assert_not_null(content);
    cr_assert_str_eq(content, "line1\n");
    free(content);
    unlink("/tmp/ms_tree_hd_input");
    unlink(out);
}

Test(tree_exec, append_twice_to_file)
{
    char *env[] = {"PATH=/bin:/usr/bin", NULL};
    char **env_ptr = env;
    char *out = "/tmp/ms_tree_append_out";
    tree_t *tree = get_tree_token("echo A > /tmp/ms_tree_append_out ; echo B >> /tmp/ms_tree_append_out");
    char *content = NULL;

    unlink(out);
    cr_assert_not_null(tree);
    cr_assert_eq(exec_tree(tree, &env_ptr), 0);
    free_tree(tree);
    content = read_file_content(out);
    cr_assert_not_null(content);
    cr_assert_str_eq(content, "A\nB\n");
    free(content);
    unlink(out);
}

Test(tree_exec, heredoc_in_script_pipe_consumes_following_lines)
{
    char *env[] = {"PATH=/bin:/usr/bin", NULL};
    char **env_ptr = env;
    int saved_stdin = dup(STDIN_FILENO);
    int fd_pipe[2] = {0};
    char *line = NULL;
    size_t len = 0;
    ssize_t size = 0;
    tree_t *tree = NULL;
    char *content = NULL;

    unlink("/tmp/ms_tree_hd_script_out");
    cr_assert_eq(pipe(fd_pipe), 0);
    write(fd_pipe[1], "cat << EOF | wc -l > /tmp/ms_tree_hd_script_out\na\nb\nEOF\n/bin/echo after\n", 72);
    close(fd_pipe[1]);
    dup2(fd_pipe[0], STDIN_FILENO);
    close(fd_pipe[0]);
    size = getline(&line, &len, stdin);
    cr_assert_gt(size, 1);
    line[size - 1] = '\0';
    tree = get_tree_token(line);
    cr_assert_not_null(tree);
    cr_assert_eq(exec_tree(tree, &env_ptr), 0);
    free_tree(tree);
    size = getline(&line, &len, stdin);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    cr_assert_gt(size, 0);
    cr_assert_str_eq(line, "/bin/echo after\n");
    content = read_file_content("/tmp/ms_tree_hd_script_out");
    cr_assert_not_null(content);
    cr_assert_not_null(strstr(content, "2"));
    free(content);
    free(line);
    unlink("/tmp/ms_tree_hd_script_out");
}
