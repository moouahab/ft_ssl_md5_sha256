#include "md5.h"

static int      word_to_hex(uint32_t word, char *dst, int size)
{
    char        hex[] = "0123456789abcdef";
    int         i;

    if (!dst || size < 8)
        return (0);
    i = 0;
    while (i < 4)
    {
        dst[i * 2]     = hex[(word >> (i * 8 + 4)) & 0xF];
        dst[i * 2 + 1] = hex[(word >> (i * 8))     & 0xF];
        i++;
    }
    return (1);
}

static int      build_hex(t_md5_ctx *ctx, char *hash, int size)
{
    if (!ctx || !hash || size < 33)
        return (0);
    if (!word_to_hex(MD5_A(ctx), hash,      size))      return (0);
    if (!word_to_hex(MD5_B(ctx), hash + 8,  size - 8))  return (0);
    if (!word_to_hex(MD5_C(ctx), hash + 16, size - 16)) return (0);
    if (!word_to_hex(MD5_D(ctx), hash + 24, size - 24)) return (0);
    hash[32] = '\0';
    return (1);
}

char            *md5(t_md5_ctx *ctx)
{
    char        *hash;

    if (!ctx)
        return (NULL);
    if (ctx->msg_size > 0x1FFFFFFFFFFFFFFF)
        return (NULL);
    hash = ft_calloc(33, 1);
    if (!hash)
        return (NULL);
    if (!md5_padding(ctx) || !md5_process(ctx) || !build_hex(ctx, hash, 33))
        return (free(hash), NULL);
    return (hash);
}