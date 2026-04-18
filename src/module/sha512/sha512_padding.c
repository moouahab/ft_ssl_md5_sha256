#include "sha512.h"

int     sha512_padding(t_sha512_ctx *ctx)
{
    uint64_t    bit_len;
    uint8_t     byte;
    int         i;

    if (!ctx)
        return (0);
    bit_len = ctx->msg_size;
    sha512_process(ctx, "\x80", 1);
    while (ctx->used != 112)
        sha512_process(ctx, "\0", 1);
    i = 7;
    while (i >= 0)
    {
        byte = 0x00;
        sha512_process(ctx, &byte, 1);
        i--;
    }
    i = 7;
    while (i >= 0)
    {
        byte = (bit_len >> (i * 8)) & 0xFF;
        sha512_process(ctx, &byte, 1);
        i--;
    }
    return (1);
}