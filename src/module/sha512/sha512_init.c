#include "sha512.h"

void    sha512_init(t_sha512_ctx *ctx)
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
    SHA512_H0(ctx) = 0x6a09e667f3bcc908ULL;
    SHA512_H1(ctx) = 0xbb67ae8584caa73bULL;
    SHA512_H2(ctx) = 0x3c6ef372fe94f82bULL;
    SHA512_H3(ctx) = 0xa54ff53a5f1d36f1ULL;
    SHA512_H4(ctx) = 0x510e527fade682d1ULL;
    SHA512_H5(ctx) = 0x9b05688c2b3e6c1fULL;
    SHA512_H6(ctx) = 0x1f83d9abfb41bd6bULL;
    SHA512_H7(ctx) = 0x5be0cd19137e2179ULL;
}