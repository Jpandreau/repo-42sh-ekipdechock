/*
** EPITECH PROJECT, 2026
** line_edit
** File description:
** interactive line editor entry point
*/

#include <ncurses.h>
#include "base.h"
#include "line_edition.h"

void line_editor_init(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    clear();
    refresh();
}

void line_editor_stop(void)
{
    endwin();
}

void line_editor_suspend(void)
{
    def_prog_mode();
    endwin();
}

void line_editor_resume(void)
{
    reset_prog_mode();
}

static int state_init(line_state_t *s, history_t *history)
{
    s->history = history;
    s->hist_idx = history == NULL ? 0 : history->size;
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
    if (status == 0)
        write(1, "\r\n", 2);
    return finalize(&s, line, len, status);
}
