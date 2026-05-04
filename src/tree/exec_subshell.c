/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** execute subshell (cmd) in a child process
*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "tree.h"

int exec_subshell(tree_t *node, char ***env, history_t *history,
    job_state_t *job)
{
    pid_t pid = fork();
    int status = 0;

    if (pid == -1)
        return 84;
    if (pid == 0)
        exit(exec_tree(node->left, env, history, job));
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status))
        return 1;
    return WEXITSTATUS(status);
}
