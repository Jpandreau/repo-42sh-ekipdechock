/*
** EPITECH PROJECT, 2026
** tokenize_line_helpers
** File description:
** tokenize_line_helpers
*/

#ifndef TOKENIZE_LINE_HELPERS
    #define TOKENIZE_LINE_HELPERS

    #include "small_headers.h"

char **fail_tokens(char **tokens, char *buf);
int add_token_str(char ***tokens, char *str);
int add_token_buf(char ***tokens, char *buf, int len);
int add_operator(char *line, tokenize_ctx_t *ctx);
int flush_token(tokenize_ctx_t *ctx);

#endif /* TOKENIZE_LINE_HELPERS */
