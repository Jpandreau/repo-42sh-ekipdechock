/*
** EPITECH PROJECT, 2026
** tokenize_line_helpers
** File description:
** tokenize_line_helpers
*/

#include "base.h"
#include "tokenize_line_helpers.h"

char **fail_tokens(char **tokens, char *buf)
{
    if (buf != NULL)
        free(buf);
    if (tokens != NULL)
        free_array(tokens);
    return NULL;
}

int add_token_str(char ***tokens, char *str)
{
    char *dup = my_strdup(str);
    char **tmp = NULL;

    if (dup == NULL)
        return 1;
    tmp = my_array_add(*tokens, dup);
    if (tmp == NULL) {
        free(dup);
        return 1;
    }
    *tokens = tmp;
    return 0;
}

int add_token_buf(char ***tokens, char *buf, int len)
{
    char *dup = NULL;
    char **tmp = NULL;

    if (len == 0)
        return 0;
    dup = my_strndup(buf, len);
    if (dup == NULL)
        return 1;
    tmp = my_array_add(*tokens, dup);
    if (tmp == NULL) {
        free(dup);
        return 1;
    }
    *tokens = tmp;
    return 0;
}

int add_operator(char *line, tokenize_ctx_t *ctx)
{
    char op[3] = {0};
    int len = 1;

    if ((line[ctx->i] == '&' && line[ctx->i + 1] == '&') ||
        (line[ctx->i] == '|' && line[ctx->i + 1] == '|') ||
        (line[ctx->i] == '>' && line[ctx->i + 1] == '>') ||
        (line[ctx->i] == '<' && line[ctx->i + 1] == '<'))
        len = 2;
    op[0] = line[ctx->i];
    if (len == 2)
        op[1] = line[ctx->i + 1];
    ctx->i += len;
    return add_token_str(&ctx->tokens, op);
}

int flush_token(tokenize_ctx_t *ctx)
{
    if (add_token_buf(&ctx->tokens, ctx->buf, ctx->len))
        return 1;
    ctx->len = 0;
    return 0;
}
