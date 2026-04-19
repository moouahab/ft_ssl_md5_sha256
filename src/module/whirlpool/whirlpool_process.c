#include "whirlpool.h"

int     whirlpool_process(t_whirlpool_ctx *ctx, const void *data, uint64_t len)
{
    const uint8_t   *bytes;
    uint64_t        fill;
    uint64_t        i;

    if (!ctx)
        return (0);
    if (!data && len > 0)
        return (0);
    if (len == 0)
        return (1);
    bytes = (const uint8_t *)data;
    i = 0;
    while (i < len)
    {
        if (ctx->used == 0 && len - i >= 64)
        {
            copy_bytes(ctx->block.bytes, (uint8_t *)(bytes + i), 64);
            whirlpool_transform(ctx);
            ctx->msg_size += 512;
            i += 64;
        }
        else
        {
            fill = 64 - ctx->used;
            if (fill > len - i)
                fill = len - i;
            copy_bytes(ctx->block.bytes + ctx->used, (uint8_t *)(bytes + i), fill);
            ctx->used += fill;
            ctx->msg_size += fill * 8;
            i += fill;
            if (ctx->used == 64)
            {
                whirlpool_transform(ctx);
                ctx->used = 0;
            }
        }
    }
    return (1);
}