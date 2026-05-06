/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** tokenize_line_helpers
*/

#ifndef TOKENIZE_LINE_HELPERS
    #define TOKENIZE_LINE_HELPERS

    #include "small_headers.h"

int add_token_str(token_t ***tokens, char *str, token_type_t type);
int add_token_buf(token_t ***tokens, char *buf, int len, token_type_t type);
int add_operator(char *line, tokenize_ctx_t *ctx);
int flush_token(tokenize_ctx_t *ctx);
void free_tokens(token_t **tokens);

#endif /* TOKENIZE_LINE_HELPERS */
