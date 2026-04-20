/*
** EPITECH PROJECT, 2026
** exec_background
** File description:
** run a tree in background
*/

#include "tree.h"
#include "job_control.h"

int exec_background(tree_t *node, char ***env, history_t *history)
{
    if (node == NULL)
        return 0;
    return job_launch_background(node, env, history);
}
