#include "algorithme.h"

void    md5_init(t_md5_ctx *ctx)
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
    ctx->used = 0;
    ctx->msg_size = 0;
    MD5_A(ctx) = 0x67452301;
    MD5_B(ctx) = 0xEFCDAB89;
    MD5_C(ctx) = 0x98BADCFE;
    MD5_D(ctx) = 0x10325476;
}
