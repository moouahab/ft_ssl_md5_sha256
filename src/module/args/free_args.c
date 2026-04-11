#include "args.h"

static void     free_files(t_args *args)
{
    int     i;

    i = 0;
    while (i < args->files_count)
    {
        if (args->files[i].fd != -1)
            close(args->files[i].fd);
        i++;
    }
    free(args->files);
}

static void     free_strings(t_args *args)
{
    free(args->strings);
}

void            ft_free_args(t_args *args)
{
    if (!args)
        return ;
    free_files(args);
    free_strings(args);
    free(args);
}