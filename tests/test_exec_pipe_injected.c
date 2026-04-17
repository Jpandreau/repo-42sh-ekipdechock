/*
** EPITECH PROJECT, 2026
** test_exec_pipe_injected
** File description:
** injected tests for hard exec_pipe branches
*/

#include <criterion/criterion.h>
#include <signal.h>
#include "tree.h"

static int g_pipe_idx = 0;
static int g_pipe_ret[8] = {0};
static int g_fork_idx = 0;
static int g_fork_ret[8] = {0};
static int g_wait_idx = 0;
static int g_wait_status[8] = {0};
static int g_kill_calls = 0;

int mocked_pipe(int fd_pipe[2])
{
    int ret = g_pipe_ret[g_pipe_idx++];

    if (ret == 0) {
        fd_pipe[0] = 100 + g_pipe_idx;
        fd_pipe[1] = 200 + g_pipe_idx;
    }
    return ret;
}

pid_t mocked_fork(void)
{
    return g_fork_ret[g_fork_idx++];
}

pid_t mocked_waitpid(pid_t pid, int *status, int options)
{
    (void)pid;
    (void)options;
    *status = g_wait_status[g_wait_idx++];
    return 1;
}

int mocked_kill(pid_t pid, int sig)
{
    (void)pid;
    (void)sig;
    g_kill_calls++;
    return 0;
}

#define exec_pipe tested_exec_pipe_injected
#define pipe mocked_pipe
#define fork mocked_fork
#define waitpid mocked_waitpid
#define kill mocked_kill
#include "../src/tree/exec_pipe.c"
#undef kill
#undef waitpid
#undef fork
#undef pipe
#undef exec_pipe

static void reset_pipe_mocks(void)
{
    g_pipe_idx = 0;
    g_fork_idx = 0;
    g_wait_idx = 0;
    g_kill_calls = 0;
    for (int i = 0; i < 8; i++) {
        g_pipe_ret[i] = 0;
        g_fork_ret[i] = 1;
        g_wait_status[i] = 0;
    }
}

static tree_t *make_pipe_tree(void)
{
    tree_t *pipe_node = new_node(TOKEN_PIPE);

    pipe_node->left = new_node(TOKEN_CMD);
    pipe_node->right = new_node(TOKEN_CMD);
    return pipe_node;
}

Test(exec_pipe_injected, success_last_exitstatus)
{
    tree_t *tree = make_pipe_tree();
    char *env_data[] = {"PATH=/bin", NULL};
    char **env = env_data;

    reset_pipe_mocks();
    g_fork_ret[0] = 111;
    g_fork_ret[1] = 222;
    g_wait_status[0] = 0;
    g_wait_status[1] = 0;
    cr_assert_eq(tested_exec_pipe_injected(tree, &env), 0);
    free_tree(tree);
}

Test(exec_pipe_injected, success_last_signaled)
{
    tree_t *tree = make_pipe_tree();
    char *env_data[] = {"PATH=/bin", NULL};
    char **env = env_data;

    reset_pipe_mocks();
    g_fork_ret[0] = 111;
    g_fork_ret[1] = 222;
    g_wait_status[0] = 0;
    g_wait_status[1] = SIGSEGV;
    cr_assert_eq(tested_exec_pipe_injected(tree, &env), 128 + SIGSEGV);
    free_tree(tree);
}

Test(exec_pipe_injected, spawn_failure_triggers_cleanup)
{
    tree_t *tree = make_pipe_tree();
    char *env_data[] = {"PATH=/bin", NULL};
    char **env = env_data;

    reset_pipe_mocks();
    g_fork_ret[0] = 111;
    g_fork_ret[1] = -1;
    cr_assert_eq(tested_exec_pipe_injected(tree, &env), 84);
    cr_assert_eq(g_kill_calls, 1);
    free_tree(tree);
}

Test(exec_pipe_injected, pipe_failure_returns_84)
{
    tree_t *tree = make_pipe_tree();
    char *env_data[] = {"PATH=/bin", NULL};
    char **env = env_data;

    reset_pipe_mocks();
    g_pipe_ret[0] = -1;
    cr_assert_eq(tested_exec_pipe_injected(tree, &env), 84);
    free_tree(tree);
}
