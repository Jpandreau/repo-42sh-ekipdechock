/*
** EPITECH PROJECT, 2026
** small_headers
** File description:
** small_headers
*/

#ifndef SMALL_HEADERS
    #define SMALL_HEADERS

    #include <stddef.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <shell.h>

typedef struct tokenize_ctx_s {
    int i;
    int len;
    int in_quote;
    char quote;
    char *buf;
    char **tokens;
} tokenize_ctx_t;

typedef struct history_s {
    char **entries;
    int size;
    int capacity;
} history_t;

char **tokenize_line(char *line);
int tokenize_step(char *line, tokenize_ctx_t *ctx);

int parse_exit_line(char *line, int *code);
int exit_code_from_args(char **args, int *valid);
int parse_exit_code_arg(char *str, int *ok);

typedef struct job_state_s job_state_t;

typedef struct exec_ctx_s {
    history_t *history;
    job_state_t *job;
    shell_t *shell;
} exec_ctx_t;

int init_exec(char **line, char ***env, exec_ctx_t *ctx);
int handle_line(char **line, char ***env, exec_ctx_t *ctx);

int history_init(history_t *history);
void history_destroy(history_t *history);
int history_add(history_t *history, char *line);
int history_expand_line(history_t *history, char *line, char **expanded);
int interactive_getline(char **line, size_t *len, history_t *history);
int history_skip_spaces(char *line);
int history_only_spaces_after(char *line, int pos);
char *history_expand_number(char *line, int idx, history_t *history);
char *history_expand_prefix(char *line, int idx, history_t *history);
char *history_resolve_bang(history_t *history, char *line, int idx);

int run_line(char *input_line, char ***env, int *exit_code, exec_ctx_t *ctx);
int open_and_stat(char *filename, struct stat *st);
int read_content(int file, struct stat *st, char **content);
int handle_pipe_line(char *input_line, char ***env, int *exit_code,
    exec_ctx_t *ctx);

#endif /* SMALL_HEADERS */
