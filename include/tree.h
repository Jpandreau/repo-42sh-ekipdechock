/*
** EPITECH PROJECT, 2026
** tree
** File description:
** tree
*/

#ifndef TREE
    #define TREE

    #include <sys/types.h>
    #include "small_headers.h"

typedef enum {
    TOKEN_CMD,
    TOKEN_PIPE,
    TOKEN_SEQUENCE,
    TOKEN_BACKGROUND,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_REDIR_LEFT,
    TOKEN_REDIR_RIGHT,
    TOKEN_REDIR_APPEND,
    TOKEN_EOF
} tree_type_t;

typedef struct tree_s {
    tree_type_t type;
    struct tree_s *left;
    struct tree_s *right;
    char *input;
    char *output;
    int append;
    int heredoc;
    int heredoc_fd;
    char **args;
} tree_t;

typedef struct pipe_exec_ctx_s {
    tree_t **cmds;
    pid_t *pids;
    int count;
    int spawned;
    int prev_read;
    int fd_pipe[2];
    char ***env;
    history_t *history;
    job_state_t *job;
} pipe_exec_ctx_t;

tree_t *parse_sequence(token_t **tokens, int *pos);
tree_t *parse_logic(token_t **tokens, int *pos);
tree_t *parse_pipe(token_t **tokens, int *pos);
tree_t *parse_command(token_t **tokens, int *pos);

tree_t *new_node(tree_type_t type);
tree_t *get_tree_token(char *line);

int exec_tree(tree_t *node, char ***env, history_t *history, job_state_t *job);
int exec_tree_nofork(tree_t *node, char ***env, history_t *history,
    job_state_t *job);
int exec_pipe(tree_t *node, char ***env, history_t *history, job_state_t *job);
int exec_background(tree_t *node, char ***env, history_t *history,
    job_state_t *job);
int exec_cmd_node_nofork(tree_t *node, char ***env, history_t *history,
    job_state_t *job);
int exec_cmd_with_redirections(tree_t *node, char ***env, history_t *history,
    job_state_t *job);
int prepare_tree_heredocs(tree_t *node);
void free_tree(tree_t *node);

int make_exit_status(int code);
int is_exit_status(int status);
int exit_status_code(int status);
int find_exit_in_pipe(tree_t *node, int *code, int *invalid);

#endif /* !TREE */
