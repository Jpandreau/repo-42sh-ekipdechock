/*
** EPITECH PROJECT, 2026
** script_loop
** File description:
** script_loop
*/

#include "my.h"
#include "base.h"
#include "tree.h"
#include "small_headers.h"
#include "shell.h"

int final_exit_script_loop(int exit_code, char *line, char **env)
{
    free(line);
    free_array(env);
    return exit_code;
}

int clean_getline(char **line, size_t *len)
{
    ssize_t size = 0;

    size = getline(line, len, stdin);
    if (size == -1) {
        if (isatty(STDIN_FILENO))
            write(1, "exit\n", 5);
        return -1;
    }
    if (size == 1)
        return 1;
    (*line)[size - 1] = '\0';
    return 0;
}

static int loop_step(char **line, size_t *len, shell_t *shell)
{
    int status = clean_getline(line, len);

    if (status == 1)
        return 0;
    if (status == -1 || *line == NULL)
        return 84;
    status = handle_line(line, &shell->env);
    if (is_exit_status(status))
        return status;
    *len = 0;
    return 0;
}

static void print_prompt(void)
{
    if (isatty(STDIN_FILENO))
        write(1, "$> ", 3);
}

static void print_exit_msg(void)
{
    if (isatty(STDIN_FILENO))
        write(1, "exit\n", 5);
}

static int handle_exit_status(int *exit_code)
{
    if (!is_exit_status(*exit_code))
        return 0;
    *exit_code = exit_status_code(*exit_code);
    print_exit_msg();
    return 1;
}

static int init_shell(shell_t *shell, char **env)
{
    if (env == NULL)
        return 84;
    shell->env = env;
    shell->locals = malloc(sizeof(char *));
    if (shell->locals == NULL)
        return 84;
    shell->locals[0] = NULL;
    shell->aliases = NULL;
    shell->exit_code = 0;
    shell->running = 1;
    return 0;
}

int script_loop(char **env)
{
    shell_t shell;
    int exit_code = 0;
    char *line = NULL;
    size_t len = 0;

    if (init_shell(&shell, env) != 0)
        return 84;
    signal(SIGINT, handle_sigint);
    while (exit_code != 84) {
        print_prompt();
        exit_code = loop_step(&line, &len, &shell);
        if (handle_exit_status(&exit_code))
            break;
        if (exit_code == 84)
            break;
    }
    free_array(shell.locals);
    return final_exit_script_loop(exit_code, line, shell.env);
}
