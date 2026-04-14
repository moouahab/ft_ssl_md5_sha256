#include "sha256.h"
#include <stdio.h>

static const uint32_t   K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static void     build_schedule(uint32_t *W, uint32_t *M)
{
    int     i;

    if (!W || !M)
        return ;
    i = 0;
    while (i < 16)
    {
        W[i] = SHA256_SWAP(M[i]);
        i++;
    }
    while (i < 64)
    {
        W[i] = SHA256_s1(W[i - 2]) + W[i - 7]
             + SHA256_s0(W[i - 15]) + W[i - 16];
        i++;
    }
}

static void     sha256_round(t_sha256_regs *r, uint32_t w, uint32_t k)
{
    uint32_t    t1;
    uint32_t    t2;

    if (!r)
        return ;
    t1 = r->h + SHA256_S1(r->e) + SHA256_CH(r->e, r->f, r->g) + k + w;
    t2 = SHA256_S0(r->a) + SHA256_MAJ(r->a, r->b, r->c);
    r->h = r->g;
    r->g = r->f;
    r->f = r->e;
    r->e = r->d + t1;
    r->d = r->c;
    r->c = r->b;
    r->b = r->a;
    r->a = t1 + t2;
}

static void     sha256_rounds(t_sha256_regs *r, uint32_t *W)
{
    int     i;

    if (!r || !W)
        return ;
    i = 0;
    while (i < 64)
    {
        sha256_round(r, W[i], K[i]);
        i++;
    }
}

void            sha256_transform(t_sha256_ctx *ctx)
{
    uint32_t        W[64];
    t_sha256_regs   r;
    int             i;

    if (!ctx)
        return ;
    build_schedule(W, ctx->block.words);
    i = 0;
    while (i < 8)
    {
        r.r[i] = ctx->h[i];
        i++;
    }
    sha256_rounds(&r, W);
    i = 0;
    while (i < 8)
    {
        ctx->h[i] += r.r[i];
        i++;
    }
}