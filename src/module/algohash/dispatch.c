#include "hash.h"

static const t_hasher   hashers[] = {
    { "md5",    "MD5",    compute_md5 },
    { NULL,     NULL,     NULL        }
};

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