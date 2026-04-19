#include "whirlpool.h"

int     whirlpool_padding(t_whirlpool_ctx *ctx)
{
    uint64_t    bit_len;
    uint8_t     byte;
    int         i;

    if (!ctx)
        return (0);
    bit_len = ctx->msg_size;
    whirlpool_process(ctx, "\x80", 1);
    while (ctx->used != 32)
    {
        if (ctx->used > 32)
            while (ctx->used != 0)
                whirlpool_process(ctx, "\0", 1);
        else
            whirlpool_process(ctx, "\0", 1);
    }
    i = 0;
    while (i < 24)
    {
        byte = 0;
        whirlpool_process(ctx, &byte, 1);
        i++;
    }
    i = 7;
    while (i >= 0)
    {
        byte = (bit_len >> (i * 8)) & 0xFF;
        whirlpool_process(ctx, &byte, 1);
        i--;
    }
    
    return (1);
}