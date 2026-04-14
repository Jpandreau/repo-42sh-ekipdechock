/*
** EPITECH PROJECT, 2026
** exec_redirection
** File description:
** execute command node with redirections
*/

#include "base.h"
#include "my.h"
#include "tree.h"
#include "buildin.h"

static void print_redirection_error(char *path)
{
    my_putstr_err(path);
    my_putstr_err(": ");
    my_putstr_err(strerror(errno));
    my_putstr_err(".\n");
}

static int write_heredoc_content(int fd_write, char *delim)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read_size = 0;

    while (1) {
        read_size = getline(&line, &len, stdin);
        if (read_size == -1)
            break;
        if (read_size > 0 && line[read_size - 1] == '\n')
            line[read_size - 1] = '\0';
        if (my_strcmp(line, delim) == 0)
            break;
        write(fd_write, line, my_strlen(line));
        write(fd_write, "\n", 1);
    }
    free(line);
    return 0;
}

static int open_heredoc_fd(char *delim)
{
    int fd_pipe[2] = {0};

    if (pipe(fd_pipe) == -1)
        return -1;
    write_heredoc_content(fd_pipe[1], delim);
    close(fd_pipe[1]);
    return fd_pipe[0];
}

static int get_input_fd(tree_t *node)
{
    if (node->heredoc && node->heredoc_fd != -1)
        return node->heredoc_fd;
    if (node->heredoc)
        return open_heredoc_fd(node->input);
    return open(node->input, O_RDONLY);
}

static int redirect_input(tree_t *node)
{
    int fd = 0;

    if (node->input == NULL)
        return 0;
    fd = get_input_fd(node);
    if (fd < 0) {
        print_redirection_error(node->input);
        return 1;
    }
    if (dup2(fd, STDIN_FILENO) == -1) {
        close(fd);
        print_redirection_error(node->input);
        return 1;
    }
    close(fd);
    if (fd == node->heredoc_fd)
        node->heredoc_fd = -1;
    return 0;
}

static int redirect_output(tree_t *node)
{
    int flags = O_CREAT | O_WRONLY;
    int fd = 0;

    if (node->output == NULL)
        return 0;
    flags |= node->append ? O_APPEND : O_TRUNC;
    fd = open(node->output, flags, 0644);
    if (fd < 0) {
        print_redirection_error(node->output);
        return 1;
    }
    if (dup2(fd, STDOUT_FILENO) == -1) {
        close(fd);
        print_redirection_error(node->output);
        return 1;
    }
    close(fd);
    return 0;
}

static int apply_redirections_node(tree_t *node)
{
    if (redirect_input(node) != 0)
        return 1;
    if (redirect_output(node) != 0)
        return 1;
    return 0;
}

static int run_command_node(tree_t *node, char ***env)
{
    if (node->args == NULL || node->args[0] == NULL)
        return 0;
    if (buildin(node->args[0]))
        return run_buildin_args(node->args, env);
    return actions_cmd_args(node->args, env);
}

static void restore_stdio(int saved_in, int saved_out)
{
    dup2(saved_in, STDIN_FILENO);
    dup2(saved_out, STDOUT_FILENO);
    close(saved_in);
    close(saved_out);
}

int exec_cmd_with_redirections(tree_t *node, char ***env)
{
    int saved_in = dup(STDIN_FILENO);
    int saved_out = dup(STDOUT_FILENO);
    int status = 0;

    if (saved_in < 0 || saved_out < 0)
        return 84;
    if (apply_redirections_node(node) != 0) {
        restore_stdio(saved_in, saved_out);
        return 1;
    }
    status = run_command_node(node, env);
    restore_stdio(saved_in, saved_out);
    return status;
}
