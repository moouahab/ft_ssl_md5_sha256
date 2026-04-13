#include "hash.h"
#include "serialize.h"
#include "md5.h"

char    *compute_md5(t_input *input)
{
    t_md5_ctx   *ctx;
    char        *hash;
    t_serial    s;
    uint64_t    offset;
    uint64_t    remaining;

    if (!input)
        return (NULL);
    ctx = ft_calloc(1, sizeof(t_md5_ctx));
    if (!ctx)
        return (NULL);
    md5_init(ctx);
    ctx->msg_size = input->size;
    offset = 0;
    while (offset < input->size)
    {
        remaining = input->size - offset;
        s.bytes      = ctx->block.bytes;
        s.used       = &ctx->used;
        s.block_size = 64;
        serialize_bytes(&s, input->data + offset, remaining);
        offset += ctx->used;
        if (ctx->used == 64)
            md5_process(ctx);
        else
            break ;
    }
    hash = md5(ctx);
    free(ctx);
    return (hash);
}