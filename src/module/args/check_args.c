#include "args.h"
#include "hash.h"

bool    check_command(t_args *args, char *command)
{
    if (!is_valid_command(command))
        return (false);
    args->command = command;
    return (true);
}

bool    check_flag(t_args *args, char *flag)
{
    if (ft_strcmp(flag, "-p") == 0)
        args->flags.p = true;
    else if (ft_strcmp(flag, "-q") == 0)
        args->flags.q = true;
    else if (ft_strcmp(flag, "-r") == 0)
        args->flags.r = true;
    else if (ft_strcmp(flag, "-s") == 0)
        args->flags.s = true;
    else
        return (false);
    return (true);
}

bool    add_file(t_args *args, char *filename)
{
    t_file  *new_files;
    int     old_cap;

    if (args->files_count == args->files_capacity)
    {
        old_cap = args->files_capacity;
        args->files_capacity = old_cap == 0 ? 4 : old_cap * 2;
        new_files = ft_realloc(args->files,
            sizeof(t_file) * old_cap,
            sizeof(t_file) * args->files_capacity);
        if (!new_files)
            return (false);
        args->files = new_files;
    }
    args->files[args->files_count].name = filename;
    args->files[args->files_count].fd = -1;
    args->files_count++;
    return (true);
}

bool    add_string(t_args *args, char *string)
{
    char    **new_strings;
    int     old_cap;

    if (args->strings_count == args->strings_capacity)
    {
        old_cap = args->strings_capacity;
        args->strings_capacity = old_cap == 0 ? 4 : old_cap * 2;
        new_strings = ft_realloc(args->strings,
            sizeof(char *) * old_cap,
            sizeof(char *) * args->strings_capacity);
        if (!new_strings)
            return (false);
        args->strings = new_strings;
    }
    args->strings[args->strings_count++] = string;
    return (true);
}