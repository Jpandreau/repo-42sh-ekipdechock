/*
** EPITECH PROJECT, 2026
** test_tree_exit_nofork_helpers
** File description:
** coverage for exit_in_pipe, exec_tree_nofork and run
*/

#include <criterion/criterion.h>
#include "base.h"
#include "my.h"
#include "tree.h"

static tree_t *make_cmd(char *a0, char *a1, char *a2)
{
    tree_t *node = new_node(TOKEN_CMD);
    int count = 1;

    if (a1 != NULL)
        count++;
    if (a2 != NULL)
        count++;
    node->args = malloc(sizeof(char *) * (count + 1));
    if (node->args == NULL)
        return node;
    node->args[0] = my_strdup(a0);
    node->args[1] = a1 ? my_strdup(a1) : NULL;
    node->args[2] = a2 ? my_strdup(a2) : NULL;
    node->args[count] = NULL;
    return node;
}

Test(find_exit_in_pipe, null_tree)
{
    int code = 99;
    int invalid = 99;

    cr_assert_eq(find_exit_in_pipe(NULL, &code, &invalid), 0);
    cr_assert_eq(code, 0);
    cr_assert_eq(invalid, 0);
}

Test(find_exit_in_pipe, exit_in_pipe_valid)
{
    tree_t *pipe_node = new_node(TOKEN_PIPE);
    int code = 0;
    int invalid = 0;

    pipe_node->left = make_cmd("echo", "ok", NULL);
    pipe_node->right = make_cmd("exit", "42", NULL);
    cr_assert_eq(find_exit_in_pipe(pipe_node, &code, &invalid), 1);
    cr_assert_eq(code, 42);
    cr_assert_eq(invalid, 0);
    free_tree(pipe_node);
}

Test(find_exit_in_pipe, exit_in_pipe_invalid_arg)
{
    tree_t *pipe_node = new_node(TOKEN_PIPE);
    int code = 0;
    int invalid = 0;

    pipe_node->left = make_cmd("echo", "ok", NULL);
    pipe_node->right = make_cmd("exit", "abc", NULL);
    cr_assert_eq(find_exit_in_pipe(pipe_node, &code, &invalid), 0);
    cr_assert_eq(code, 0);
    cr_assert_eq(invalid, 1);
    free_tree(pipe_node);
}

Test(find_exit_in_pipe, exit_outside_pipe_not_detected)
{
    tree_t *node = make_cmd("exit", "7", NULL);
    int code = 0;
    int invalid = 0;

    cr_assert_eq(find_exit_in_pipe(node, &code, &invalid), 0);
    cr_assert_eq(code, 0);
    cr_assert_eq(invalid, 0);
    free_tree(node);
}

Test(exec_tree_nofork, null_and_unknown_node)
{
    char *env_data[] = {"PATH=/bin:/usr/bin", NULL};
    char **env = env_data;
    tree_t *node = new_node(TOKEN_EOF);

    cr_assert_eq(exec_tree_nofork(NULL, &env), 0);
    cr_assert_eq(exec_tree_nofork(node, &env), 1);
    free_tree(node);
}

Test(exec_tree_nofork, and_or_sequence_paths)
{
    char *env_data[] = {"PATH=/bin:/usr/bin", "A=1", NULL};
    char **env = env_data;
    tree_t *and_node = new_node(TOKEN_AND);
    tree_t *or_node = new_node(TOKEN_OR);
    tree_t *seq_node = new_node(TOKEN_SEQUENCE);

    and_node->left = make_cmd("env", NULL, NULL);
    and_node->right = new_node(TOKEN_EOF);
    cr_assert_eq(exec_tree_nofork(and_node, &env), 1);
    or_node->left = new_node(TOKEN_EOF);
    or_node->right = new_node(TOKEN_EOF);
    cr_assert_eq(exec_tree_nofork(or_node, &env), 1);
    seq_node->left = new_node(TOKEN_EOF);
    seq_node->right = new_node(TOKEN_EOF);
    cr_assert_eq(exec_tree_nofork(seq_node, &env), 1);
    free_tree(and_node);
    free_tree(or_node);
    free_tree(seq_node);
}

Test(run_helper, run_true_does_not_crash)
{
    char *args[] = {"/bin/true", NULL};
    char *env[] = {"PATH=/bin:/usr/bin", NULL};

    run("/bin/true", args, env);
    cr_assert(1);
}
