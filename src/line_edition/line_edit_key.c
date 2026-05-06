/*
** EPITECH PROJECT, 2026
** line_edit_key
** File description:
** key handling and input loop
*/

#include <unistd.h>
#include <errno.h>
#include "base.h"
#include "line_edition.h"

static int read_escape_seq3(void)
{
    unsigned char tilde = 0;

    if (read(STDIN_FILENO, &tilde, 1) == 1 && tilde == '~')
        return LE_KEY_DEL;
    return 27;
}

static int read_escape(void)
{
    unsigned char seq[2] = {0};

    if (read(STDIN_FILENO, &seq[0], 1) != 1)
        return 27;
    if (read(STDIN_FILENO, &seq[1], 1) != 1)
        return 27;
    if (seq[0] != '[')
        return 27;
    if (seq[1] == 'A')
        return LE_KEY_UP;
    if (seq[1] == 'B')
        return LE_KEY_DOWN;
    if (seq[1] == 'C')
        return LE_KEY_RIGHT;
    if (seq[1] == 'D')
        return LE_KEY_LEFT;
    if (seq[1] == '3')
        return read_escape_seq3();
    return 27;
}

static int read_char(void)
{
    unsigned char c = 0;
    ssize_t n = read(STDIN_FILENO, &c, 1);

    if (n < 0 && errno == EINTR)
        return -2;
    if (n <= 0)
        return -1;
    if ((int)c == 27)
        return read_escape();
    return (int)c;
}

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

static int handle_delete(line_state_t *s)
{
    int i = s->pos;

    if (s->pos >= s->len)
        return 0;
    s->len--;
    for (; i < s->len; i++)
        s->buffer[i] = s->buffer[i + 1];
    s->buffer[s->len] = '\0';
    return 0;
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
    if (ch == 127 || ch == 8)
        return handle_backspace(s);
    if (ch == LE_KEY_LEFT && s->pos > 0)
        s->pos--;
    if (ch == LE_KEY_RIGHT && s->pos < s->len)
        s->pos++;
    if (ch == LE_KEY_DEL)
        return handle_delete(s);
    if (ch == LE_KEY_UP)
        return handle_history(s, 1);
    if (ch == LE_KEY_DOWN)
        return handle_history(s, 0);
    if (ch >= 32 && ch < 127)
        return handle_char(s, ch);
    return 0;
}

int read_loop(line_state_t *s)
{
    int ch = 0;
    int status = 0;

    while (1) {
        ch = read_char();
        if (ch == '\r' || ch == '\n') {
            write(STDOUT_FILENO, "\r\n", 2);
            return 0;
        }
        if (ch == 4 || ch == -1)
            return -1;
        if (ch == -2)
            return 1;
        status = handle_key(s, ch);
        if (status == 84)
            return 84;
        redraw_line(s);
    }
}
