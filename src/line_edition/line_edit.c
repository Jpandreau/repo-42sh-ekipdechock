/*
** EPITECH PROJECT, 2026
** line_edit
** File description:
** interactive line editor entry point
*/

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include "base.h"
#include "line_edition.h"

static void set_raw_mode(void)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

static void set_cooked_mode(void)
{
    struct termios cooked;

    tcgetattr(STDIN_FILENO, &cooked);
    cooked.c_lflag |= (ECHO | ICANON);
    cooked.c_iflag |= ICRNL;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &cooked);
}

void line_editor_init(void)
{
    set_raw_mode();
}

void line_editor_stop(void)
{
    set_cooked_mode();
}

void line_editor_suspend(void)
{
    set_cooked_mode();
}

void line_editor_resume(void)
{
    set_raw_mode();
}

static int state_init(line_state_t *s, history_t *history)
{
    s->history = history;
    s->hist_idx = history == NULL ? 0 : history->size;
    s->prev_len = 0;
    return state_load(s, "");
}

static int finalize(line_state_t *s, char **line, size_t *len, int status)
{
    if (status != 0) {
        state_free(s);
        if (status == -1)
            return -1;
        return status == 84 ? 84 : 1;
    }
    *line = s->buffer;
    if (len != NULL)
        *len = (size_t)s->len;
    free(s->saved);
    return 0;
}

int interactive_getline(char **line, size_t *len, history_t *history)
{
    line_state_t s = {0};
    int status = 0;

    if (line == NULL)
        return 84;
    *line = NULL;
    if (state_init(&s, history) == 84)
        return 84;
    redraw_line(&s);
    status = read_loop(&s);
    return finalize(&s, line, len, status);
}
