/*
** EPITECH PROJECT, 2026
** prepare_heredoc
** File description:
** preload here-doc content before forking
*/

#include "base.h"
#include "tree.h"
#include <stdio.h>

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

static int prepare_cmd_heredoc(tree_t *node)
{
    if (node->type != TOKEN_CMD || !node->heredoc)
        return 0;
    if (node->input == NULL || node->heredoc_fd != -1)
        return 0;
    node->heredoc_fd = open_heredoc_fd(node->input);
    if (node->heredoc_fd == -1)
        return 84;
    return 0;
}

int prepare_tree_heredocs(tree_t *node)
{
    if (node == NULL)
        return 0;
    if (prepare_tree_heredocs(node->left) != 0)
        return 84;
    if (prepare_cmd_heredoc(node) != 0)
        return 84;
    return prepare_tree_heredocs(node->right);
}
