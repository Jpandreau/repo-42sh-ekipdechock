/*
** EPITECH PROJECT, 2026
** line_edit_key
** File description:
** key handling and input loop
*/

#include <ncurses.h>
#include <errno.h>
#include "base.h"
#include "line_edition.h"

static int history_save(line_state_t *s)
{
    if (s->saved != NULL)
        return 0;
    s->saved = my_strdup(s->buffer == NULL ? "" : s->buffer);
    return s->saved == NULL ? 84 : 0;
}

static int handle_history(line_state_t *s, int up)
{
    if (s->history == NULL || s->history->size == 0)
        return 0;
    if (history_save(s) == 84)
        return 84;
    if (up && s->hist_idx > 0)
        s->hist_idx--;
    if (!up && s->hist_idx < s->history->size)
        s->hist_idx++;
    if (s->hist_idx >= s->history->size)
        return state_load(s, s->saved);
    return state_load(s, s->history->entries[s->hist_idx]);
}

static int handle_backspace(line_state_t *s)
{
    int i = s->pos;

    if (s->pos == 0)
        return 0;
    s->pos--;
    s->len--;
    for (; i <= s->len; i++)
        s->buffer[i - 1] = s->buffer[i];
    s->buffer[s->len] = '\0';
    return 0;
}

static int handle_char(line_state_t *s, int ch)
{
    int i = s->len;

    if (s->len + 1 >= s->capacity && state_grow(s) == 84)
        return 84;
    for (; i > s->pos; i--)
        s->buffer[i] = s->buffer[i - 1];
    s->buffer[s->pos] = (char)ch;
    s->pos++;
    s->len++;
    s->buffer[s->len] = '\0';
    return 0;
}

static int handle_key(line_state_t *s, int ch)
{
    if (ch == KEY_BACKSPACE || ch == 127)
        return handle_backspace(s);
    if (ch == KEY_LEFT && s->pos > 0) {
        s->pos--;
        return 0;
    }
    if (ch == KEY_RIGHT && s->pos < s->len) {
        s->pos++;
        return 0;
    }
    if (ch == KEY_UP)
        return handle_history(s, 1);
    if (ch == KEY_DOWN)
        return handle_history(s, 0);
    if (ch >= 32 && ch < 127)
        return handle_char(s, ch);
    return 0;
}

static int dispatch_key(line_state_t *s, int ch)
{
    int status = 0;

    if (ch == ERR) {
        s->len = 0;
        s->pos = 0;
        s->buffer[0] = '\0';
        return 0;
    }
    status = handle_key(s, ch);
    return status;
}

int read_loop(line_state_t *s)
{
    int ch = 0;
    int status = 0;

    while (1) {
        ch = getch();
        if (ch == '\n' || ch == '\r')
            return 0;
        if (ch == 4)
            return -1;
        if (ch == ERR && errno != EINTR)
            return -1;
        status = dispatch_key(s, ch);
        if (status == 84)
            return 84;
        redraw_line(s);
    }
}
