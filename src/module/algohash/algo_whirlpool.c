#include "hash.h"
#include "whirlpool.h"

char    *compute_whirlpool(t_input *input)
{
    t_whirlpool_ctx *ctx;
    char            *hash;

    if (!input)
        return (NULL);
    ctx = (t_whirlpool_ctx *)ft_calloc(1, sizeof(t_whirlpool_ctx));
    if (!ctx)
        return (NULL);
    whirlpool_init(ctx);
    whirlpool_process(ctx, input->data, input->size);
    hash = whirlpool(ctx);
    free(ctx);
    return (hash);
}