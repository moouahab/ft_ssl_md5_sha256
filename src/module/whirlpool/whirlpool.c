#include "whirlpool.h"

static void     byte_to_hex(uint8_t byte, char *buf)
{
    char    hex[] = "0123456789abcdef";

    buf[0] = hex[byte >> 4];
    buf[1] = hex[byte & 0xF];
}

char    *whirlpool(t_whirlpool_ctx *ctx)
{
    char    *digest;
    int     i;
    int     j;
    int     idx;

    if (!ctx)
        return (NULL);
    if (!whirlpool_padding(ctx))
        return (NULL);
    digest = (char *)ft_calloc(129, sizeof(char));
    if (!digest)
        return (NULL);
    i = 0;
    while (i < 8)
    {
        j = 0;
        while (j < 8)
        {
            idx = i * 16 + j * 2;
            byte_to_hex(ctx->state.m[i][j], digest + idx);
            j++;
        }
        i++;
    }
    digest[128] = '\0';
    return (digest);
}