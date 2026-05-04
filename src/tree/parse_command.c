/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** parse_command
*/

#include "base.h"
#include "tree.h"

static int is_control_token(char *token)
{
    if (token == NULL)
        return 1;
    if (my_strcmp(token, "|") == 0 || my_strcmp(token, "&&") == 0)
        return 1;
    if (my_strcmp(token, "||") == 0 || my_strcmp(token, ";") == 0)
        return 1;
    if (my_strcmp(token, "&") == 0)
        return 1;
    if (my_strcmp(token, ")") == 0)
        return 1;
    return 0;
}

static int is_redirection_token(char *token)
{
    if (my_strcmp(token, "<") == 0 || my_strcmp(token, "<<") == 0)
        return 1;
    if (my_strcmp(token, ">") == 0 || my_strcmp(token, ">>") == 0)
        return 1;
    return 0;
}

static char **init_args(void)
{
    char **args = malloc(sizeof(char *) * 1);

    if (args == NULL)
        return NULL;
    args[0] = NULL;
    return args;
}

static int update_arg_types(tree_t *node, token_type_t type)
{
    int size = array_size(node->args);
    token_type_t *new_types = malloc(sizeof(token_type_t) * size);
    int i = 0;

    if (new_types == NULL)
        return 1;
    while (i < size - 1) {
        if (node->arg_types != NULL)
            new_types[i] = node->arg_types[i];
        else
            new_types[i] = TOKEN_TYPE_WORD;
        i++;
    }
    new_types[size - 1] = type;
    free(node->arg_types);
    node->arg_types = new_types;
    return 0;
}

static int add_arg_with_type(tree_t *node, char *token, token_type_t type)
{
    char *dup = my_strdup(token);
    char **tmp = NULL;

    if (dup == NULL)
        return 1;
    tmp = my_array_add(node->args, dup);
    if (tmp == NULL) {
        free(dup);
        return 1;
    }
    node->args = tmp;
    if (update_arg_types(node, type) != 0)
        return 1;
    return 0;
}

static int set_redir_field(char **field, char *token)
{
    char *dup = my_strdup(token);

    if (dup == NULL)
        return 1;
    if (*field != NULL)
        free(*field);
    *field = dup;
    return 0;
}

static int consume_redirection(tree_t *node, token_t **tokens, int *pos)
{
    char *op = tokens[*pos]->value;

    (*pos)++;
    if (tokens[*pos] == NULL || is_control_token(tokens[*pos]->value))
        return 1;
    if (my_strcmp(op, "<") == 0) {
        node->heredoc = 0;
        return set_redir_field(&node->input, tokens[*pos]->value);
    }
    if (my_strcmp(op, "<<") == 0) {
        node->heredoc = 1;
        return set_redir_field(&node->input, tokens[*pos]->value);
    }
    node->append = my_strcmp(op, ">>") == 0;
    return set_redir_field(&node->output, tokens[*pos]->value);
}

static int consume_command_token(tree_t *node, token_t **tokens, int *pos)
{
    if (is_redirection_token(tokens[*pos]->value)) {
        if (consume_redirection(node, tokens, pos) != 0)
            return 1;
        (*pos)++;
        return 0;
    }
    if (add_arg_with_type(node, tokens[*pos]->value, tokens[*pos]->type) != 0)
        return 1;
    (*pos)++;
    return 0;
}

tree_t *parse_command(token_t **tokens, int *pos)
{
    tree_t *node = new_node(TOKEN_CMD);

    if (node == NULL)
        return NULL;
    node->args = init_args();
    if (node->args == NULL) {
        free_tree(node);
        return NULL;
    }
    while (tokens[*pos] && !is_control_token(tokens[*pos]->value))
        if (consume_command_token(node, tokens, pos) != 0) {
            free_tree(node);
            return NULL;
        }
    if (node->args[0] == NULL && node->input == NULL
        && node->output == NULL) {
        free_tree(node);
        return NULL;
    }
    return node;
}
