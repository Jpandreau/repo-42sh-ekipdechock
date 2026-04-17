/*
** EPITECH PROJECT, 2026
** run
** File description:
** run
*/

#include "my.h"

void run(char *command, char **argument, char **env)
{
    pid_t pid = fork();
    int status;

    if (pid == -1)
        exit(1);
    if (pid == 0) {
        execve(command, argument, env);
        exit(1);
    } else {
        waitpid(pid, &status, 0);
    }
}
