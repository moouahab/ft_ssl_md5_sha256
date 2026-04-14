#include "sha256.h"

/**
 * sha256_padding
 * @ctx: Le contexte SHA256
 * * 1. Ajoute le bit '1' (octet 0x80).
 * 2. Ajoute des 0x00 jusqu'à atteindre l'octet 56 du bloc.
 * 3. Ajoute la taille du message (en bits) sur les 8 derniers octets.
 */
int     sha256_padding(t_sha256_ctx *ctx)
{
    uint64_t    bit_len;
    uint8_t     byte;
    int         i;

    if (!ctx)
        return (0);
    bit_len = ctx->msg_size;
    sha256_process(ctx, "\x80", 1);
    while (ctx->used != 56)
        sha256_process(ctx, "\0", 1);
    i = 7;
    while (i >= 0)
    {
        byte = (bit_len >> (i * 8)) & 0xFF;
        sha256_process(ctx, &byte, 1);
        i--;
    }
    return (1);
}