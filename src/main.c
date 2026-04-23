/*
** EPITECH PROJECT, 2026
** main
** File description:
** main
*/

#include "my.h"
#include "base.h"
#include "small_headers.h"
#include "buildin.h"
#include "shell.h"

static char **add_default_nlspath(char **env)
{
    char *nlspath =
        "NLSPATH=/usr/share/locale/%L/LC_MESSAGES/%N.cat:/usr/share/"
        "locale/%l/LC_MESSAGES/%N.cat";
    char *value = NULL;

    if (my_getenv("NLSPATH", env) != NULL)
        return env;
    value = my_strdup(nlspath);
    if (value == NULL)
        return env;
    return my_array_add(env, value);
}

static int init_shell_env(shell_t *shell, char **env)
{
    shell->env = env;
    shell->locals = malloc(sizeof(char *));
    if (shell->locals == NULL)
        return 84;
    shell->locals[0] = NULL;
    shell->aliases = malloc(sizeof(char *));
    if (shell->aliases == NULL) {
        free(shell->locals);
        return 84;
    }
    shell->aliases[0] = NULL;
    shell->exit_code = 0;
    shell->running = 1;
    return 0;
}

int exec_all_cmd_file(char *content, char **env)
{
    shell_t shell;
    int exit_code = 0;
    char *input_line = NULL;

    if (init_shell_env(&shell, env) != 0)
        return 84;
    for (input_line = strtok(content, "\n"); input_line != NULL;
        input_line = strtok(NULL, "\n")) {
        if (input_line[0] == '\n')
            continue;
        if (run_line(input_line, &shell, &exit_code))
            break;
    }
    free_array(shell.locals);
    return exit_code;
}

int command_file(char *filename, char **env)
{
    int exit_code = 0;
    int file = 0;
    char *content = NULL;
    struct stat st;

    file = open_and_stat(filename, &st);
    if (file == -1)
        return 84;
    if (read_content(file, &st, &content) != 0)
        return 84;
    exit_code = exec_all_cmd_file(content, env);
    free(content);
    free_array(env);
    return exit_code;
}

int pipe_input(char **env)
{
    shell_t shell;
    char *input_line = NULL;
    size_t len = 0;
    int exit_code = 0;

    if (init_shell_env(&shell, env) != 0)
        return 84;
    while (getline(&input_line, &len, stdin) != -1) {
        if (handle_pipe_line(input_line, &shell, &exit_code))
            break;
    }
    free(input_line);
    free_array(shell.locals);
    free_array(shell.env);
    return exit_code;
}

int main(int ac, char **av, char **env)
{
    char **my_env = NULL;

    if (env == NULL)
        return 84;
    signal(SIGINT, handle_sigint);
    my_env = my_array_dup(env);
    if (my_env == NULL)
        return 84;
    my_env = add_default_nlspath(my_env);
    if (my_env == NULL)
        return 84;
    if (ac > 1)
        return command_file(av[1], my_env);
    if (!isatty(STDIN_FILENO))
        return pipe_input(my_env);
    return script_loop(my_env);
}
