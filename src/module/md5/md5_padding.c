#include "md5.h"

static void     write_size(t_md5_ctx *ctx)
{
    uint64_t    bits;

    if (!ctx)
        return ;
    if (ctx->msg_size > 0x1FFFFFFFFFFFFFFF)
        return ;
    bits = ctx->msg_size * 8;
    ctx->block.words[14] = (uint32_t)(bits);
    ctx->block.words[15] = (uint32_t)(bits >> 32);
}

int     md5_padding(t_md5_ctx *ctx)
{
    if (!ctx)
        return (0);
    if (ctx->used >= 64)
    {
        if (!md5_process(ctx))
            return (0);
        if (ctx->used != 0)
            return (0);
    }
    ctx->block.bytes[ctx->used] = 0x80;
    ctx->used++;
    if (ctx->used > 56)
    {
        if (!md5_process(ctx))
            return (0);
        if (ctx->used != 0)
            return (0);
    }
    write_size(ctx);
    return (1);
}