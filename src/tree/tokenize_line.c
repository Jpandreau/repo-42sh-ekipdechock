/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** tokenize_line
*/

#include "base.h"
#include "small_headers.h"
#include "tokenize_line_helpers.h"

static token_t **fail_tokens(token_t **tokens, char *buf)
{
    if (buf != NULL)
        free(buf);
    free_tokens(tokens);
    return NULL;
}

static int init_context(tokenize_ctx_t *ctx, char *line)
{
    ctx->i = 0;
    ctx->len = 0;
    ctx->in_quote = 0;
    ctx->quote = 0;
    ctx->cur_type = TOKEN_TYPE_WORD;
    ctx->tokens = NULL;
    ctx->buf = NULL;
    ctx->tokens = malloc(sizeof(token_t *) * 1);
    ctx->buf = malloc(my_strlen(line) + 1);
    if (ctx->tokens == NULL || ctx->buf == NULL)
        return 1;
    ctx->tokens[0] = NULL;
    return 0;
}

static token_t **finalize_tokens(tokenize_ctx_t *ctx)
{
    int err = add_token_buf(&ctx->tokens, ctx->buf,
        ctx->len, ctx->cur_type);

    if (ctx->in_quote || err)
        return fail_tokens(ctx->tokens, ctx->buf);
    free(ctx->buf);
    return ctx->tokens;
}

token_t **tokenize_line(char *line)
{
    tokenize_ctx_t ctx;

    if (init_context(&ctx, line))
        return fail_tokens(ctx.tokens, ctx.buf);
    while (line[ctx.i]) {
        if (tokenize_step(line, &ctx))
            return fail_tokens(ctx.tokens, ctx.buf);
    }
    return finalize_tokens(&ctx);
}
