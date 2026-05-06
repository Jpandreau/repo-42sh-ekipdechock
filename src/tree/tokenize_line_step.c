/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** tokenizelinestep
*/

#include "tokenize_line_helpers.h"
#include "small_headers.h"

static void consume_quote(char *line, tokenize_ctx_t *ctx)
{
    if (line[ctx->i] == ctx->quote) {
        ctx->in_quote = 0;
    } else {
        ctx->buf[ctx->len] = line[ctx->i];
        ctx->len++;
    }
    ctx->i++;
}

static int consume_escape(char *line, tokenize_ctx_t *ctx)
{
    if (line[ctx->i] != '\\')
        return 0;
    if (line[ctx->i + 1] == '\0') {
        ctx->i++;
        return 1;
    }
    ctx->buf[ctx->len] = line[ctx->i + 1];
    ctx->len++;
    ctx->i += 2;
    return 1;
}

static void append_char(char *line, tokenize_ctx_t *ctx)
{
    ctx->buf[ctx->len] = line[ctx->i];
    ctx->len++;
    ctx->i++;
}

static int handle_in_quote(char *line, tokenize_ctx_t *ctx)
{
    if (!ctx->in_quote)
        return 0;
    if (ctx->quote != '\'' && consume_escape(line, ctx))
        return 1;
    consume_quote(line, ctx);
    return 1;
}

static int handle_space(char *line, tokenize_ctx_t *ctx)
{
    if (line[ctx->i] != ' ' && line[ctx->i] != '\t')
        return 0;
    if (flush_token(ctx))
        return -1;
    ctx->i++;
    return 1;
}

static int is_operator_char(char c)
{
    return c == '|' || c == ';' || c == '&'
        || c == '<' || c == '>' || c == '(' || c == ')';
}

static int handle_operator(char *line, tokenize_ctx_t *ctx)
{
    if (!is_operator_char(line[ctx->i]))
        return 0;
    if (flush_token(ctx))
        return -1;
    if (add_operator(line, ctx))
        return -1;
    return 1;
}

static token_type_t quote_to_type(char quote)
{
    if (quote == '\'')
        return TOKEN_TYPE_SINGLE_QUOTE;
    if (quote == '"')
        return TOKEN_TYPE_DOUBLE_QUOTE;
    return TOKEN_TYPE_BACKTICK;
}

static int handle_quote_start(char *line, tokenize_ctx_t *ctx)
{
    if (line[ctx->i] != '\'' && line[ctx->i] != '"'
        && line[ctx->i] != '`')
        return 0;
    if (ctx->len == 0)
        ctx->cur_type = quote_to_type(line[ctx->i]);
    ctx->in_quote = 1;
    ctx->quote = line[ctx->i];
    ctx->i++;
    return 1;
}

int tokenize_step(char *line, tokenize_ctx_t *ctx)
{
    int res;

    if (handle_in_quote(line, ctx))
        return 0;
    if (consume_escape(line, ctx))
        return 0;
    if (handle_quote_start(line, ctx))
        return 0;
    res = handle_space(line, ctx);
    if (res)
        return res < 0;
    res = handle_operator(line, ctx);
    if (res != 0)
        return res < 0;
    append_char(line, ctx);
    return 0;
}
