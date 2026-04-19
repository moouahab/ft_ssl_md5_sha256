#include "hash.h"

static t_hasher     hashers[] = {
    {"md5", "MD5", compute_md5},
    {"sha256", "SHA2-256", compute_sha256},
    {"sha512", "SHA2-512", compute_sha512},
    {"whirlpool", "WHIRLPOOL", compute_whirlpool},
    {NULL, NULL, NULL}
};

bool    is_valid_command(char *command)
{
    int     i;

    if (!command)
        return (false);
    i = 0;
    while (hashers[i].name)
    {
        if (ft_strcmp(command, hashers[i].name) == 0)
            return (true);
        i++;
    }
    return (false);
}

void    print_commands(void)
{
    int     i;

    i = 0;
    while (hashers[i].name)
    {
        ft_putstr(hashers[i].name, 2);
        ft_putstr("\n", 2);
        i++;
    }
}

void    dispatch(t_args *args)
{
    int     i;

    if (!args)
        return ;
    i = 0;
    while (hashers[i].name)
    {
        if (ft_strcmp(args->command, hashers[i].name) == 0)
        {
            run_hash(args, (t_hasher *)&hashers[i]);
            return ;
        }
        i++;
    }
}