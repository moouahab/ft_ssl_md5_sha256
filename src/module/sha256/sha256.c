#include "sha256.h"
#include "utils.h"

static void     word_to_hex(uint32_t word, char *buf)
{
    char    hex[] = "0123456789abcdef";
    int     i;

    i = 7;
    while (i >= 0)
    {
        buf[i] = hex[word & 0xF];
        word >>= 4;
        i--;
    }
}

char    *sha256(t_sha256_ctx *ctx)
{
    char    *digest;
    int     i;

    if (!ctx)
        return (NULL);
    sha256_padding(ctx);
    digest = ft_calloc(1, 65);
    if (!digest)
        return (NULL);
    i = 0;
    while (i < 8)
    {
        word_to_hex(ctx->h[i], digest + (i * 8));
        i++;
    }
    digest[64] = '\0';
    return (digest);
}