#include "sha256.h"
#include "utils.h"

int     sha256_process(t_sha256_ctx *ctx, const void *data, uint64_t len)
{
    const uint8_t   *ptr;
    uint64_t        fill;

    if (!ctx || (!data && len > 0))
        return (0);
    ptr = (const uint8_t *)data;
    ctx->msg_size += len * 8;
    if (ctx->used > 0)
    {
        fill = 64 - ctx->used;
        if (len < fill)
        {
            copy_bytes(ctx->block.bytes + ctx->used, (uint8_t *)ptr, (int)len);
            ctx->used += len;
            return (1);
        }
        copy_bytes(ctx->block.bytes + ctx->used, (uint8_t *)ptr, (int)fill);
        sha256_transform(ctx);
        ptr += fill;
        len -= fill;
        ctx->used = 0;
    }
    while (len >= 64)
    {
        copy_bytes(ctx->block.bytes, (uint8_t *)ptr, 64);
        sha256_transform(ctx);
        ptr += 64;
        len -= 64;
    }
    if (len > 0)
    {
        copy_bytes(ctx->block.bytes, (uint8_t *)ptr, (int)len);
        ctx->used = len;
    }
    return (1);
}