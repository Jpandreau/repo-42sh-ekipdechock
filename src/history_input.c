/*
** EPITECH PROJECT, 2026
** history_input
** File description:
** interactive input without readline
*/

#include "base.h"
#include "small_headers.h"

#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

struct term_state_s {
    struct termios old_mode;
    struct termios raw_mode;
    int enabled;
};

struct input_state_s {
    history_t *history;
    char *buffer;
    char *saved;
    int browse_index;
    size_t len;
    size_t capacity;
};

static int input_resize(struct input_state_s *state, const char *source)
{
    size_t source_len = source == NULL ? 0 : my_strlen(source);
    char *new_buffer = realloc(state->buffer, source_len + 1);

    if (new_buffer == NULL)
        return 84;
    if (source_len > 0)
        memcpy(new_buffer, source, source_len);
    new_buffer[source_len] = '\0';
    state->buffer = new_buffer;
    state->len = source_len;
    state->capacity = source_len + 1;
    return 0;
}

static int input_redraw(struct input_state_s *state)
{
    if (write(1, "\r\033[2K", 5) == -1)
        return 84;
    if (write(1, "$> ", 3) == -1)
        return 84;
    if (state->buffer != NULL && write(1, state->buffer, state->len) == -1)
        return 84;
    return 0;
}

static int input_nav(struct input_state_s *state, int up)
{
    if (state->history == NULL || state->history->size == 0)
        return 0;
    if (state->saved == NULL) {
        state->saved = my_strdup(state->buffer == NULL ? "" : state->buffer);
        if (state->saved == NULL) {
            free(state->buffer);
            state->buffer = NULL;
            return 84;
        }
    }
    if (up && state->browse_index > 0)
        state->browse_index--;
    if (!up && state->browse_index < state->history->size)
        state->browse_index++;
    if (state->browse_index >= state->history->size)
        return input_resize(state, state->saved);
    return input_resize(state, state->history->entries[state->browse_index]);
}

static int input_handle_escape(struct input_state_s *state)
{
    char seq[2] = {0};
    int status = 0;

    if (read(STDIN_FILENO, &seq[0], 1) != 1
        || read(STDIN_FILENO, &seq[1], 1) != 1 || seq[0] != '[')
        return 0;
    if (seq[1] == 'A')
        status = input_nav(state, 1);
    else if (seq[1] == 'B')
        status = input_nav(state, 0);
    if (status == 84)
        return 84;
    if (state->history != NULL && state->browse_index >= state->history->size
        && state->saved != NULL) {
        free(state->saved);
        state->saved = NULL;
    }
    return input_redraw(state);
}

static int input_push_char(struct input_state_s *state, char ch)
{
    if (state->len + 1 >= state->capacity) {
        state->capacity = state->capacity == 0 ? 16 : state->capacity * 2;
        state->buffer = realloc(state->buffer, state->capacity);
        if (state->buffer == NULL)
            return 84;
    }
    state->buffer[state->len] = ch;
    state->len++;
    state->buffer[state->len] = '\0';
    return input_redraw(state);
}

static int input_handle_char(struct input_state_s *state, char ch)
{
    if (ch == '\n' || ch == '\r')
        return 2;
    if (ch == 127 || ch == '\b') {
        if (state->len == 0)
            return 1;
        state->len--;
        state->buffer[state->len] = '\0';
        return input_redraw(state);
    }
    if (ch == 27)
        return input_handle_escape(state);
    if (!isprint((unsigned char)ch) && ch != '\t')
        return 1;
    return input_push_char(state, ch);
}

static int input_read_loop(struct input_state_s *state)
{
    int status = 0;
    ssize_t rd = 0;
    char ch = '\0';

    while (1) {
        rd = read(STDIN_FILENO, &ch, 1);
        if (rd == 0)
            return -1;
        if (rd == -1)
            return errno == EINTR ? 1 : 84;
        status = input_handle_char(state, ch);
        if (status == 2)
            return 0;
        if (status == 84)
            return 84;
    }
}

static int input_finalize(struct input_state_s *state, char **line, size_t *len,
    int status)
{
    free(state->saved);
    if (status == 84 || status == -1) {
        free(state->buffer);
        return status;
    }
    if (status == 1 || state->len == 0) {
        free(state->buffer);
        return 1;
    }
    *line = state->buffer;
    if (len != NULL)
        *len = state->len;
    return 0;
}

static int input_prepare_session(struct input_state_s *input,
    struct term_state_s *term, history_t *history)
{
    input->history = history;
    input->browse_index = history == NULL ? 0 : history->size;
    if (input_resize(input, "") == 84)
        return 84;
    if (tcgetattr(STDIN_FILENO, &term->old_mode) == -1) {
        free(input->buffer);
        return 84;
    }
    term->raw_mode = term->old_mode;
    term->raw_mode.c_lflag &= ~(ICANON | ECHO);
    term->raw_mode.c_cc[VMIN] = 1;
    term->raw_mode.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term->raw_mode) == -1) {
        free(input->buffer);
        return 84;
    }
    term->enabled = 1;
    input_redraw(input);
    return 0;
}

int interactive_get_line(char **line, size_t *len, history_t *history)
{
    struct term_state_s term = {0};
    struct input_state_s input = {0};
    int status = 0;

    if (line == NULL)
        return 84;
    *line = NULL;
    if (input_prepare_session(&input, &term, history) == 84)
        return 84;
    status = input_read_loop(&input);
    if (term.enabled)
        tcsetattr(STDIN_FILENO, TCSANOW, &term.old_mode);
    if (status == 0)
        write(1, "\n", 1);
    return input_finalize(&input, line, len, status);
}
