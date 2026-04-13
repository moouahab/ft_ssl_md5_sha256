#include "args.h"
#include "hash.h"

int     main(int ac, char **av)
{
    t_args  *args;

    args = parse_args(ac, av);
    if (!args)
        return (1);
    dispatch(args);
    ft_free_args(args);
    return (0);
}