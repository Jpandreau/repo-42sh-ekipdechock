/*
** EPITECH PROJECT, 2026
** job_control
** File description:
** minimal background/foreground job control
*/

#include "my.h"
#include "job_control.h"
#include "base.h"
#include "tree.h"

static job_state_t read_job_state(job_state_t *job)
{
    job_state_t state = {0};

    if (job == NULL)
        return state;
    if (job->active == 0 || job->pgid <= 0)
        return state;
    state.pgid = job->pgid;
    state.active = 1;
    state.stopped = job->stopped;
    return state;
}

static void write_job_state(job_state_t *state, job_state_t *job)
{
    if (job == NULL)
        return;
    if (state->active == 0 || state->pgid <= 0) {
        job->pgid = 0;
        job->active = 0;
        job->stopped = 0;
        return;
    }
    job->pgid = state->pgid;
    job->active = 1;
    job->stopped = state->stopped;
}

static int mark_stopped_job(job_state_t *state, job_state_t *job)
{
    state->stopped = 1;
    tcsetpgrp(STDIN_FILENO, getpgrp());
    write_job_state(state, job);
    return 148;
}

static void run_background_child(tree_t *node, char ***env, exec_ctx_t *ctx)
{
    int status = 0;

    setpgid(0, 0);
    signal(SIGINT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    status = exec_tree_nofork(node, env, ctx);
    exit(status);
}

static int wait_foreground_job(job_state_t *state, job_state_t *job)
{
    int status = 0;
    int code = 0;

    while (waitpid(-state->pgid, &status, WUNTRACED) > 0) {
        if (WIFSTOPPED(status))
            return mark_stopped_job(state, job);
        if (WIFEXITED(status))
            code = WEXITSTATUS(status);
        if (WIFSIGNALED(status))
            code = 128 + WTERMSIG(status);
    }
    tcsetpgrp(STDIN_FILENO, getpgrp());
    state->active = 0;
    write_job_state(state, job);
    return code;
}

void job_control_init(void)
{
    if (!isatty(STDIN_FILENO))
        return;
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    tcsetpgrp(STDIN_FILENO, getpgrp());
}

void job_control_reap(job_state_t *job)
{
    job_state_t state = read_job_state(job);
    int status = 0;
    pid_t pid = 0;

    if (!state.active)
        return;
    pid = waitpid(-state.pgid, &status, WNOHANG);
    while (pid > 0)
        pid = waitpid(-state.pgid, &status, WNOHANG);
    if (pid == -1 && errno == ECHILD)
        state.active = 0;
    write_job_state(&state, job);
}

int job_launch_background(tree_t *node, char ***env, exec_ctx_t *ctx)
{
    pid_t pid = fork();
    job_state_t state = {0};

    if (pid == -1)
        return 84;
    if (pid == 0)
        run_background_child(node, env, ctx);
    setpgid(pid, pid);
    state.pgid = pid;
    state.active = 1;
    state.stopped = 0;
    write_job_state(&state, ctx->job);
    printf("[1] %d\n", (int)pid);
    return 0;
}

int fg_buildin_args(char **args, job_state_t *job)
{
    job_state_t state;

    if (args == NULL || args[0] == NULL || args[1] != NULL)
        return 1;
    job_control_reap(job);
    state = read_job_state(job);
    if (!state.active) {
        my_putstr_err("fg: no current job.\n");
        return 1;
    }
    if (tcsetpgrp(STDIN_FILENO, state.pgid) == -1 && errno != ENOTTY)
        return 1;
    if (kill(-state.pgid, SIGCONT) == -1 && errno != ESRCH) {
        tcsetpgrp(STDIN_FILENO, getpgrp());
        return 1;
    }
    state.stopped = 0;
    write_job_state(&state, job);
    return wait_foreground_job(&state, job);
}

int bg_buildin_args(char **args, job_state_t *job)
{
    job_state_t state;

    if (args == NULL || args[0] == NULL || args[1] != NULL)
        return 1;
    job_control_reap(job);
    state = read_job_state(job);
    if (!state.active) {
        my_putstr_err("bg: no current job.\n");
        return 1;
    }
    if (kill(-state.pgid, SIGCONT) == -1)
        return 1;
    state.stopped = 0;
    write_job_state(&state, job);
    return 0;
}
