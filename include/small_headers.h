/*
** EPITECH PROJECT, 2026
** small_headers
** File description:
** small_headers
*/

#ifndef SMALL_HEADERS
    #define SMALL_HEADERS

    #include <sys/stat.h>

typedef struct tokenize_ctx_s {
    int i;
    int len;
    int in_quote;
    char quote;
    char *buf;
    char **tokens;
} tokenize_ctx_t;

char **tokenize_line(char *line);
int tokenize_step(char *line, tokenize_ctx_t *ctx);

int parse_exit_line(char *line, int *code);
int exit_code_from_args(char **args, int *valid);
int parse_exit_code_arg(char *str, int *ok);

int init_exec(char **line, char ***env);
int handle_line(char **line, char ***env);

int run_line(char *input_line, char ***env, int *exit_code);
int open_and_stat(char *filename, struct stat *st);
int read_content(int file, struct stat *st, char **content);
int handle_pipe_line(char *input_line, char ***env, int *exit_code);

#endif /* SMALL_HEADERS */
