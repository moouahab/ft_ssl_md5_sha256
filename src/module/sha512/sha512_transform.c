#include "sha512.h"

static const uint64_t   K[80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL,
    0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
    0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
    0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
    0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
    0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL,
    0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
    0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
    0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
    0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL,
    0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL,
    0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
    0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
    0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL,
    0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL,
    0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
    0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
    0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
    0xd192e819d6ef5218ULL, 0xd69906245565a910ULL,
    0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
    0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
    0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
    0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
    0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL,
    0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL,
    0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
    0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
    0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
    0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
    0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL,
    0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
    0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
    0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

static void     build_schedule(uint64_t *W, uint64_t *M)
{
    int     i;

    if (!W || !M)
        return ;
    i = 0;
    while (i < 16)
    {
        W[i] = SHA512_SWAP(M[i]);
        i++;
    }
    while (i < 80)
    {
        W[i] = SHA512_s1(W[i - 2]) + W[i - 7]
             + SHA512_s0(W[i - 15]) + W[i - 16];
        i++;
    }
}

static void     sha512_round(t_sha512_regs *r, uint64_t w, uint64_t k)
{
    uint64_t    t1;
    uint64_t    t2;

    if (!r)
        return ;
    t1 = r->h + SHA512_S1(r->e) + SHA512_CH(r->e, r->f, r->g) + k + w;
    t2 = SHA512_S0(r->a) + SHA512_MAJ(r->a, r->b, r->c);
    r->h = r->g;
    r->g = r->f;
    r->f = r->e;
    r->e = r->d + t1;
    r->d = r->c;
    r->c = r->b;
    r->b = r->a;
    r->a = t1 + t2;
}

static void     sha512_rounds(t_sha512_regs *r, uint64_t *W)
{
    int     i;

    if (!r || !W)
        return ;
    i = 0;
    while (i < 80)
    {
        sha512_round(r, W[i], K[i]);
        i++;
    }
}

void            sha512_transform(t_sha512_ctx *ctx)
{
    uint64_t        W[80];
    t_sha512_regs   r;
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
    sha512_rounds(&r, W);
    i = 0;
    while (i < 8)
    {
        ctx->h[i] += r.r[i];
        i++;
    }
}
