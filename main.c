#include "args.h"
#include "utils.h"

static void     print_flags(t_flags flags)
{
    ft_putstr("Flags   :\n", 1);
    ft_putstr("  -p : ", 1);
    ft_putstr(flags.p ? "true\n" : "false\n", 1);
    ft_putstr("  -q : ", 1);
    ft_putstr(flags.q ? "true\n" : "false\n", 1);
    ft_putstr("  -r : ", 1);
    ft_putstr(flags.r ? "true\n" : "false\n", 1);
    ft_putstr("  -s : ", 1);
    ft_putstr(flags.s ? "true\n" : "false\n", 1);
}

static void     print_strings(t_args *args)
{
    int     i;

    ft_putstr("Strings :\n", 1);
    i = 0;
    while (i < args->strings_count)
    {
        ft_putstr("  [", 1);
        ft_putstr(args->strings[i], 1);
        ft_putstr("]\n", 1);
        i++;
    }
}

static void     print_files(t_args *args)
{
    int     i;

    ft_putstr("Files   :\n", 1);
    i = 0;
    while (i < args->files_count)
    {
        ft_putstr("  [", 1);
        ft_putstr(args->files[i].name, 1);
        ft_putstr("]\n", 1);
        i++;
    }
}

int     main(int ac, char **av)
{
    t_args  *args;

    args = parse_args(ac, av);
    if (!args)
        return (1);

    
    ft_free_args(args);
    return (0);
}