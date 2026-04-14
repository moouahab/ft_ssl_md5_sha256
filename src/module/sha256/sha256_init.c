#include "sha256.h"

void    sha256_init(t_sha256_ctx *ctx)
{
    int     i;

    if (!ctx)
        return ;
    i = 0;
    while (i < 16)
    {
        ctx->block.words[i] = 0;
        i++;
    }
    ctx->used     = 0;
    ctx->msg_size = 0;
    SHA256_H0(ctx) = 0x6a09e667;
    SHA256_H1(ctx) = 0xbb67ae85;
    SHA256_H2(ctx) = 0x3c6ef372;
    SHA256_H3(ctx) = 0xa54ff53a;
    SHA256_H4(ctx) = 0x510e527f;
    SHA256_H5(ctx) = 0x9b05688c;
    SHA256_H6(ctx) = 0x1f83d9ab;
    SHA256_H7(ctx) = 0x5be0cd19;
}