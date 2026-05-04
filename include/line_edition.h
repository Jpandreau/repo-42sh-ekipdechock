/*
** EPITECH PROJECT, 2026
** line_edition
** File description:
** line_edition
*/

#ifndef LINE_EDITION_H
    #define LINE_EDITION_H

    #include "small_headers.h"

typedef struct line_state_s {
    char *buffer;
    int pos;
    int len;
    int capacity;
    history_t *history;
    int hist_idx;
    char *saved;
    int last_cursor_abs;
} line_state_t;

void line_editor_init(void);
void line_editor_stop(void);
void line_editor_suspend(void);
void line_editor_resume(void);
int state_grow(line_state_t *s);
int state_load(line_state_t *s, char *src);
void state_free(line_state_t *s);
void redraw_line(line_state_t *s);
int read_loop(line_state_t *s);

#endif /* LINE_EDITION_H */
