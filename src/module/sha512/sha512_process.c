#include "sha512.h"
#include "utils.h"

int     sha512_process(t_sha512_ctx *ctx, const void *data, uint64_t len)
{
    const uint8_t   *ptr;
    uint64_t        fill;

    if (!ctx || (!data && len > 0))
        return (0);
    ptr = (const uint8_t *)data;
    ctx->msg_size += len * 8;
    if (ctx->used > 0)
    {
        fill = 128 - ctx->used;
        if (len < fill)
        {
            copy_bytes(ctx->block.bytes + ctx->used, (uint8_t *)ptr, (int)len);
            ctx->used += len;
            return (1);
        }
        copy_bytes(ctx->block.bytes + ctx->used, (uint8_t *)ptr, (int)fill);
        sha512_transform(ctx);
        ptr += fill;
        len -= fill;
        ctx->used = 0;
    }
    while (len >= 128)
    {
        copy_bytes(ctx->block.bytes, (uint8_t *)ptr, 128);
        sha512_transform(ctx);
        ptr += 128;
        len -= 128;
    }
    if (len > 0)
    {
        copy_bytes(ctx->block.bytes, (uint8_t *)ptr, (int)len);
        ctx->used = len;
    }
    return (1);
}