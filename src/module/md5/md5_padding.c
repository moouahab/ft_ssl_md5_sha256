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

void            md5_padding(t_md5_ctx *ctx)
{
    if (!ctx)
        return ;
    if (ctx->used >= 64)
    {
        md5_process(ctx);
        if (ctx->used != 0)
            return ;
    }
    ctx->block.bytes[ctx->used] = 0x80;
    ctx->used++;
    if (ctx->used > 56)
    {
        md5_process(ctx);
        if (ctx->used != 0)
            return ;
    }
    write_size(ctx);
}