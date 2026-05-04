/*
** EPITECH PROJECT, 2026
** job_control
** File description:
** background and foreground job helpers
*/

#ifndef JOB_CONTROL_H
    #define JOB_CONTROL_H

    #include <sys/types.h>
    #include "small_headers.h"

typedef struct tree_s tree_t;

typedef struct job_state_s {
    pid_t pgid;
    int active;
    int stopped;
} job_state_t;

void job_control_init(void);
void job_control_reap(job_state_t *job);
int job_launch_background(tree_t *node, char ***env, exec_ctx_t *ctx);
int fg_buildin_args(char **args, job_state_t *job);
int bg_buildin_args(char **args, job_state_t *job);

#endif /* !JOB_CONTROL_H */
