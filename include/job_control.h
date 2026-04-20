/*
** EPITECH PROJECT, 2026
** job_control
** File description:
** background and foreground job helpers
*/

#ifndef JOB_CONTROL_H
    #define JOB_CONTROL_H

    #include <sys/types.h>

typedef struct tree_s tree_t;
typedef struct history_s history_t;

typedef struct job_state_s {
    pid_t pgid;
    int active;
    int stopped;
} job_state_t;

void job_control_init(void);
void job_control_reap(history_t *history);
int job_launch_background(tree_t *node, char ***env, history_t *history);
int fg_buildin_args(char **args, history_t *history);
int bg_buildin_args(char **args, history_t *history);

#endif /* !JOB_CONTROL_H */
