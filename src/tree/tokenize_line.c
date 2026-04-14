/*
** EPITECH PROJECT, 2026
** tokenize_line
** File description:
** tokenize_line
*/

#include "base.h"
#include "small_headers.h"
#include "tokenize_line_helpers.h"

static int init_context(tokenize_ctx_t *ctx, char *line)
{
    ctx->i = 0;
    ctx->len = 0;
    ctx->in_quote = 0;
    ctx->quote = 0;
    ctx->tokens = NULL;
    ctx->buf = NULL;
    ctx->tokens = malloc(sizeof(char *) * 1);
    ctx->buf = malloc(my_strlen(line) + 1);
    if (ctx->tokens == NULL || ctx->buf == NULL)
        return 1;
    ctx->tokens[0] = NULL;
    return 0;
}

static char **finalize_tokens(tokenize_ctx_t *ctx)
{
    if (ctx->in_quote || add_token_buf(&ctx->tokens, ctx->buf, ctx->len))
        return fail_tokens(ctx->tokens, ctx->buf);
    free(ctx->buf);
    return ctx->tokens;
}

char **tokenize_line(char *line)
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
