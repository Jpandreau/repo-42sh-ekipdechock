/*
** EPITECH PROJECT, 2026
** test_hooks_unit
** File description:
** Behavioral unit tests for run_hook and check_and_run_cwdcmd
*/

#include <criterion/criterion.h>
#include <unistd.h>
#include <stdlib.h>
#include "base.h"
#include "small_headers.h"
#include "shell.h"
#include "job_control.h"

Test(hooks_unit, run_hook_null_ctx_no_crash)
{
    char **env = calloc(1, sizeof(char *));

    cr_assert_not_null(env);
    run_hook("precmd", &env, NULL);
    free(env);
}

Test(hooks_unit, run_hook_no_alias_no_crash)
{
    shell_t shell = {NULL, NULL};
    history_t hist = {0};
    job_state_t job = {0};
    exec_ctx_t ctx = {&hist, &job, &shell};
    char **env = calloc(1, sizeof(char *));

    cr_assert_not_null(env);
    run_hook("precmd", &env, &ctx);
    free(env);
}

Test(hooks_unit, run_hook_executes_alias_set_var)
{
    shell_t shell = {NULL, NULL};
    history_t hist = {0};
    job_state_t job = {0};
    exec_ctx_t ctx = {&hist, &job, &shell};
    char **env = calloc(1, sizeof(char *));

    cr_assert_not_null(env);
    shell_init(&shell);
    set_alias_entry(&shell.aliases, "precmd", "set hookvar ran");
    run_hook("precmd", &env, &ctx);
    cr_assert_str_eq(get_local(shell.locals, "hookvar"), "ran");
    shell_destroy(&shell);
    free(env);
}

Test(hooks_unit, check_cwdcmd_null_old_cwd_no_crash)
{
    char **env = calloc(1, sizeof(char *));

    cr_assert_not_null(env);
    check_and_run_cwdcmd(NULL, &env, NULL);
    free(env);
}

Test(hooks_unit, check_cwdcmd_same_dir_no_hook)
{
    shell_t shell = {NULL, NULL};
    history_t hist = {0};
    job_state_t job = {0};
    exec_ctx_t ctx = {&hist, &job, &shell};
    char **env = calloc(1, sizeof(char *));
    char *cwd = getcwd(NULL, 0);

    cr_assert_not_null(env);
    cr_assert_not_null(cwd);
    shell_init(&shell);
    set_alias_entry(&shell.aliases, "cwdcmd", "set cwdcmd_fired yes");
    check_and_run_cwdcmd(cwd, &env, &ctx);
    cr_assert_null(get_local(shell.locals, "cwdcmd_fired"));
    shell_destroy(&shell);
    free(cwd);
    free(env);
}

Test(hooks_unit, check_cwdcmd_different_dir_fires_hook)
{
    shell_t shell = {NULL, NULL};
    history_t hist = {0};
    job_state_t job = {0};
    exec_ctx_t ctx = {&hist, &job, &shell};
    char **env = calloc(1, sizeof(char *));

    cr_assert_not_null(env);
    shell_init(&shell);
    set_alias_entry(&shell.aliases, "cwdcmd", "set cwdcmd_fired yes");
    check_and_run_cwdcmd("/nonexistent/old/path", &env, &ctx);
    cr_assert_str_eq(get_local(shell.locals, "cwdcmd_fired"), "yes");
    shell_destroy(&shell);
    free(env);
}
