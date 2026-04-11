#ifndef ARGS_H
#define ARGS_H

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"

typedef struct  s_flags
{
    bool    p;
    bool    q;
    bool    r;
    bool    s;
}               t_flags;

typedef struct s_file
{
    char    *name;
    int     fd;
}               t_file;

typedef struct  s_args
{
    t_flags     flags;

    char        *command;
    t_file      *files;
    
    int         files_count;
    int         files_capacity;
    
    char        **strings;
    int         strings_count;
    int         strings_capacity;
}               t_args;

bool    check_command(t_args *args, char *command);
bool    check_flag(t_args *args, char *flag);
bool    add_file(t_args *args, char *filename);
bool    add_string(t_args *args, char *string);

t_args  *parse_args(int ac, char **av);
void    ft_free_args(t_args *args);

#endif /* ARGS_H */