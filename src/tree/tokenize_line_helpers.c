/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** tokenize_line_helpers
*/

#include "base.h"
#include "tokenize_line_helpers.h"

static token_t **token_array_add(token_t **array, token_t *tok)
{
    token_t **new_array = NULL;
    int size = 0;
    int i = 0;

    if (array == NULL || tok == NULL)
        return NULL;
    while (array[size] != NULL)
        size++;
    new_array = malloc(sizeof(token_t *) * (size + 2));
    if (new_array == NULL)
        return NULL;
    while (i < size) {
        new_array[i] = array[i];
        i++;
    }
    new_array[size] = tok;
    new_array[size + 1] = NULL;
    free(array);
    return new_array;
}

static token_t *make_token(char *value, token_type_t type)
{
    token_t *tok = malloc(sizeof(token_t));

    if (tok == NULL) {
        free(value);
        return NULL;
    }
    tok->value = value;
    tok->type = type;
    return tok;
}

int add_token_str(token_t ***tokens, char *str, token_type_t type)
{
    token_t *tok = make_token(my_strdup(str), type);
    token_t **tmp = NULL;

    if (tok == NULL)
        return 1;
    tmp = token_array_add(*tokens, tok);
    if (tmp == NULL) {
        free(tok->value);
        free(tok);
        return 1;
    }
    *tokens = tmp;
    return 0;
}

int add_token_buf(token_t ***tokens, char *buf, int len, token_type_t type)
{
    token_t *tok = NULL;
    token_t **tmp = NULL;

    if (len == 0)
        return 0;
    tok = make_token(my_strndup(buf, len), type);
    if (tok == NULL)
        return 1;
    tmp = token_array_add(*tokens, tok);
    if (tmp == NULL) {
        free(tok->value);
        free(tok);
        return 1;
    }
    *tokens = tmp;
    return 0;
}

static token_type_t get_operator_type(char c)
{
    if (c == '(')
        return TOKEN_TYPE_SUBSHELL_OPEN;
    if (c == ')')
        return TOKEN_TYPE_SUBSHELL_CLOSE;
    return TOKEN_TYPE_OPERATOR;
}

int add_operator(char *line, tokenize_ctx_t *ctx)
{
    token_type_t type = get_operator_type(line[ctx->i]);
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
    return add_token_str(&ctx->tokens, op, type);
}

int flush_token(tokenize_ctx_t *ctx)
{
    if (add_token_buf(&ctx->tokens, ctx->buf, ctx->len, ctx->cur_type))
        return 1;
    ctx->len = 0;
    ctx->cur_type = TOKEN_TYPE_WORD;
    return 0;
}

void free_tokens(token_t **tokens)
{
    int i = 0;

    if (tokens == NULL)
        return;
    while (tokens[i] != NULL) {
        free(tokens[i]->value);
        free(tokens[i]);
        i++;
    }
    free(tokens);
}
