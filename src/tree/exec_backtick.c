/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** Backtick command substitution
*/

#include <unistd.h>
#include <sys/wait.h>
#include "base.h"
#include "small_headers.h"

static void run_child(char *cmd, int fd_write, char **env)
{
    char *argv[4];

    argv[0] = "/bin/sh";
    argv[1] = "-c";
    argv[2] = cmd;
    argv[3] = NULL;
    dup2(fd_write, STDOUT_FILENO);
    close(fd_write);
    execve("/bin/sh", argv, env);
    exit(84);
}

static char *read_pipe_output(int fd)
{
    char *buf = malloc(4097);
    ssize_t n = 0;
    int total = 0;

    if (buf == NULL)
        return NULL;
    while (total < 4096) {
        n = read(fd, buf + total, 4096 - total);
        if (n <= 0)
            break;
        total += (int)n;
    }
    buf[total] = '\0';
    return buf;
}

static void strip_trailing_newlines(char *str)
{
    int len = my_strlen(str);

    while (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
        len--;
    }
}

static void newlines_to_spaces(char *str)
{
    int i = 0;

    while (str[i]) {
        if (str[i] == '\n')
            str[i] = ' ';
        i++;
    }
}

static char *fork_and_capture(char *cmd, char **env)
{
    int fd_pipe[2];
    pid_t pid = 0;
    char *output = NULL;

    if (pipe(fd_pipe) == -1)
        return NULL;
    pid = fork();
    if (pid == -1) {
        close(fd_pipe[0]);
        close(fd_pipe[1]);
        return NULL;
    }
    if (pid == 0)
        run_child(cmd, fd_pipe[1], env);
    close(fd_pipe[1]);
    output = read_pipe_output(fd_pipe[0]);
    close(fd_pipe[0]);
    waitpid(pid, NULL, 0);
    return output;
}

char **expand_backtick(char *cmd, char ***env)
{
    char *output = NULL;
    char **result = NULL;

    if (cmd == NULL)
        return NULL;
    output = fork_and_capture(cmd, *env);
    if (output == NULL)
        return NULL;
    strip_trailing_newlines(output);
    newlines_to_spaces(output);
    result = my_str_to_word_array(output, ' ');
    free(output);
    return result;
}
