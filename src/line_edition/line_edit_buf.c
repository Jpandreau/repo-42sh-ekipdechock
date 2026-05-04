/*
** EPITECH PROJECT, 2026
** line_edit_buf
** File description:
** line buffer management
*/

#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "line_edition.h"

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
    int back = s->len - s->pos;
    int i = 0;

    write(1, "\r\033[2K$> ", 8);
    if (s->buffer != NULL && s->len > 0)
        write(1, s->buffer, s->len);
    for (i = 0; i < back; i++)
        write(1, "\033[D", 3);
}
