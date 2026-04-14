/*
** EPITECH PROJECT, 2026
** get_tree_token
** File description:
** get_tree_token
*/

#include "base.h"
#include "tree.h"
#include "small_headers.h"

tree_t *get_tree_token(char *line)
{
    char **tokens = tokenize_line(line);
    int pos = 0;
    tree_t *tree = NULL;

    if (tokens == NULL || tokens[0] == NULL) {
        free_array(tokens);
        return NULL;
    }
    tree = parse_sequence(tokens, &pos);
    if (tokens[pos] != NULL) {
        free_tree(tree);
        tree = NULL;
    }
    free_array(tokens);
    return tree;
}
