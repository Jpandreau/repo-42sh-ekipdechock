/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** get_tree_token
*/

#include "base.h"
#include "tree.h"
#include "small_headers.h"
#include "tokenize_line_helpers.h"

tree_t *get_tree_token(char *line)
{
    token_t **tokens = tokenize_line(line);
    int pos = 0;
    tree_t *tree = NULL;

    if (tokens == NULL || tokens[0] == NULL) {
        free_tokens(tokens);
        return NULL;
    }
    tree = parse_sequence(tokens, &pos);
    if (tokens[pos] != NULL) {
        free_tree(tree);
        tree = NULL;
    }
    free_tokens(tokens);
    return tree;
}
