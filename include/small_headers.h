/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** smallheaders
*/

#ifndef SMALL_HEADERS_H
    #define SMALL_HEADERS_H

    #include <stddef.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <shell.h>

typedef enum {
    TOKEN_TYPE_WORD,
    TOKEN_TYPE_SINGLE_QUOTE,
    TOKEN_TYPE_DOUBLE_QUOTE,
    TOKEN_TYPE_BACKTICK,
    TOKEN_TYPE_GLOB_STAR,
    TOKEN_TYPE_GLOB_QUESTION,
    TOKEN_TYPE_GLOB_BRACKET,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_SUBSHELL_OPEN,
    TOKEN_TYPE_SUBSHELL_CLOSE,
} token_type_t;

typedef struct token_s {
    token_type_t type;
    char *value;
} token_t;

typedef struct tokenize_ctx_s {
    int i;
    int len;
    int in_quote;
    char quote;
    char *buf;
    token_t **tokens;
    token_type_t cur_type;
} tokenize_ctx_t;

typedef struct history_s {
    char **entries;
    int size;
    int capacity;
} history_t;

typedef struct job_state_s job_state_t;

typedef struct exec_ctx_s {
    history_t *history;
    job_state_t *job;
    shell_t *shell;
} exec_ctx_t;

token_t **tokenize_line(char *line);
int tokenize_step(char *line, tokenize_ctx_t *ctx);
int parse_exit_line(char *line, int *code);
int exit_code_from_args(char **args, int *valid);
int parse_exit_code_arg(char *str, int *ok);
int init_exec(char **line, char ***env, exec_ctx_t *ctx);
int handle_line(char **line, char ***env, exec_ctx_t *ctx);
int init_shell_ctx(history_t *history, shell_t *shell);
int history_init(history_t *history);
void history_destroy(history_t *history);
int history_add(history_t *history, char *line);
int history_expand_line(history_t *history,
    char *line, char **expanded);

int interactive_get_line(char **line,
    size_t *len, history_t *history);
int history_skip_spaces(char *line);
int history_only_spaces_after(char *line, int pos);
char *history_expand_number(char *line,
    int idx, history_t *history);
char *history_expand_prefix(char *line,
    int idx, history_t *history);
char *history_resolve_bang(history_t *history,
    char *line, int idx);
int run_line(char *input_line, char ***env,
    int *exit_code, exec_ctx_t *ctx);
int open_and_stat(char *filename, struct stat *st);
int read_content(int file, struct stat *st, char **content);
int handle_pipeline(char *input_line, char ***env,
    int *exit_code, exec_ctx_t *ctx);
void run_hook(char *name, char ***env, exec_ctx_t *ctx);
void check_and_run_cwdcmd(char *old_cwd, char ***env, exec_ctx_t *ctx);

#endif
