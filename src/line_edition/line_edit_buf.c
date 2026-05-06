/*
** EPITECH PROJECT, 2026
** line_edit_buf
** File description:
** line buffer management
*/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "line_edition.h"

static void write_spaces(int n)
{
    char buf[64];
    int chunk = 0;

    while (n > 0) {
        chunk = n > 64 ? 64 : n;
        memset(buf, ' ', chunk);
        write(STDOUT_FILENO, buf, chunk);
        n -= chunk;
    }
}

int state_grow(line_state_t *s)
{
    int new_cap = s->capacity == 0 ? 16 : s->capacity * 2;
    char *new_buf = realloc(s->buffer, new_cap);

    if (new_buf == NULL)
        return 84;
    s->buffer = new_buf;
    s->capacity = new_cap;
    return 0;
}

int state_load(line_state_t *s, char *src)
{
    int src_len = src == NULL ? 0 : (int)my_strlen(src);
    char *new_buf = realloc(s->buffer, src_len + 1);

    if (new_buf == NULL)
        return 84;
    if (src_len > 0)
        memcpy(new_buf, src, src_len);
    new_buf[src_len] = '\0';
    s->buffer = new_buf;
    s->len = src_len;
    s->pos = src_len;
    s->capacity = src_len + 1;
    return 0;
}

void state_free(line_state_t *s)
{
    free(s->saved);
    free(s->buffer);
}

void redraw_line(line_state_t *s)
{
    int display_len = 3 + s->len;
    int erase = s->prev_len > display_len ? s->prev_len - display_len : 0;

    write(STDOUT_FILENO, "\r$> ", 4);
    if (s->buffer != NULL && s->len > 0)
        write(STDOUT_FILENO, s->buffer, s->len);
    write_spaces(erase);
    write(STDOUT_FILENO, "\r$> ", 4);
    if (s->pos > 0)
        write(STDOUT_FILENO, s->buffer, s->pos);
    s->prev_len = display_len;
}
