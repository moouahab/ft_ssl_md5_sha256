#include "args.h"
#include "hash.h"

static void     print_usage(void)
{
    ft_putstr("usage: ft_ssl command [flags] [file/string]\n", 2);
}

static void     print_invalid(char *cmd)
{
    ft_putstr("ft_ssl: Error: '", 2);
    ft_putstr(cmd, 2);
    ft_putstr("' is an invalid command.\n\nCommands:\n", 2);
    print_commands();
    ft_putstr("\nFlags:\n-p -q -r -s\n", 2);
}

static void     print_unknown_flag(t_args *args, char *flag)
{
    ft_putstr("ft_ssl: ", 2);
    ft_putstr(args->command, 2);
    ft_putstr(": unknown option -- ", 2);
    ft_putstr(flag + 1, 2);
    ft_putstr("\n", 2);
}

static bool     parse_flags(t_args *args, char **av, int *i)
{
    bool    files_started;

    files_started = false;
    while (av[*i])
    {
        if (!files_started && av[*i][0] == '-')
        {
            if (!check_flag(args, av[*i]))
            {
                print_unknown_flag(args, av[*i]);
                return (false);
            }
            if (args->flags.s)
            {
                (*i)++;
                if (!av[*i])
                    return (ft_putstr("ft_ssl: Error: '-s' requires an argument.\n", 2), false);
                if (!add_string(args, av[*i]))
                    return (false);
                args->flags.s = false;
            }
        }
        else
        {
            files_started = true;
            if (!add_file(args, av[*i]))
                return (false);
        }
        (*i)++;
    }
    return (true);
}

t_args          *parse_args(int ac, char **av)
{
    t_args  *args;
    int     i;

    if (ac < 2)
        return (print_usage(), NULL);
    args = ft_calloc(1, sizeof(t_args));
    if (!args)
        return (NULL);
    if (!check_command(args, av[1]))
        return (print_invalid(av[1]), ft_free_args(args), NULL);
    i = 2;
    if (!parse_flags(args, av, &i))
        return (ft_free_args(args), NULL);
    return (args);
}