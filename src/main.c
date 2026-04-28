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
#include "job_control.h"

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

int exec_all_cmd_file(char *content, char ***env)
{
    int exit_code = 0;
    char *input_line = NULL;
    history_t history = {0};
    job_state_t job = {0};
    exec_ctx_t ctx = {&history, &job};

    if (history_init(&history) == 84)
        return 84;
    for (input_line = strtok(content, "\n"); input_line != NULL;
        input_line = strtok(NULL, "\n")) {
        if (input_line[0] == '\n')
            continue;
        if (run_line(input_line, env, &exit_code, &ctx))
            break;
    }
    history_destroy(&history);
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
    exit_code = exec_all_cmd_file(content, &env);
    free(content);
    free_array(env);
    return exit_code;
}

int pipe_input(char **env)
{
    char *input_line = NULL;
    size_t len = 0;
    int exit_code = 0;
    history_t history = {0};
    job_state_t job = {0};
    exec_ctx_t ctx = {&history, &job};

    if (history_init(&history) == 84) {
        free_array(env);
        return 84;
    }
    while (getline(&input_line, &len, stdin) != -1) {
        if (handle_pipeline(input_line, &env, &exit_code, &ctx))
            break;
    }
    history_destroy(&history);
    free(input_line);
    free_array(env);
    return exit_code;
}

int main(int ac, char **av, char **env)
{
    char **my_env = NULL;

    if (env == NULL)
        return 84;
    signal(SIGINT, handle_sigint);
    job_control_init();
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
