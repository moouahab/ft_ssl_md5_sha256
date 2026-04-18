#include "sha512.h"
#include "utils.h"

static void     word_to_hex64(uint64_t word, char *buf)
{
    char    hex[] = "0123456789abcdef";
    int     i;

    i = 15;
    while (i >= 0)
    {
        buf[i] = hex[word & 0xF];
        word >>= 4;
        i--;
    }
}

char    *sha512(t_sha512_ctx *ctx)
{
    char    *digest;
    int     i;

    if (!ctx)
        return (NULL);
    sha512_padding(ctx);
    digest = ft_calloc(1, 129);
    if (!digest)
        return (NULL);
    i = 0;
    while (i < 8)
    {
        word_to_hex64(ctx->h[i], digest + (i * 16));
        i++;
    }
    digest[128] = '\0';
    return (digest);
}