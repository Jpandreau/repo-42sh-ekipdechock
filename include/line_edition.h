/*
** EPITECH PROJECT, 2026
** line_edition
** File description:
** line_edition
*/

#ifndef LINE_EDITION_H
    #define LINE_EDITION_H

    #include "small_headers.h"

    #define LE_KEY_UP 1001
    #define LE_KEY_DOWN 1002
    #define LE_KEY_RIGHT 1003
    #define LE_KEY_LEFT 1004
    #define LE_KEY_DEL 1005

typedef struct line_state_s {
    char *buffer;
    int pos;
    int len;
    int capacity;
    history_t *history;
    int hist_idx;
    char *saved;
    int prev_len;
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
int interactive_getline(char **line, size_t *len, history_t *history);

#endif /* LINE_EDITION_H */
