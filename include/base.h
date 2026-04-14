/*
** EPITECH PROJECT, 2026
** base
** File description:
** base
*/

#ifndef BASE
    #define BASE
    #include <stdlib.h>
    #include <unistd.h>

void my_putchar(char c);
void my_putstr(char const *str);
void my_putstr_err(char const *str);

char *my_strdup(char const *str);
char *my_strndup(char const *str, int n);
int my_strlen(char const *str);
char *my_strcat(char *dest, char const *src);
int my_strcmp(const char *s1, const char *s2);
int my_strncmp(char const *s1, char const *s2, int n);
char **my_str_to_word_array(char const *str, char const delim);
int is_sep(char c, char delim);

int free_array(char **array);
void free_line(char **line);
int array_size(char **array);
char **my_array_dup(char **array);
char **my_array_add(char **array, char *new_str);

#endif /* !BASE */
