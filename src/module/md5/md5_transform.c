#include "md5.h"

static const uint32_t   T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static const uint32_t   S[64] = {
    7, 12, 17, 22,  7, 12, 17, 22,
    7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,
    5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,
    4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,
    6, 10, 15, 21,  6, 10, 15, 21
};

static int      check_rounds(uint32_t *a, uint32_t *b,
                                uint32_t *c, uint32_t *d, uint32_t *M)
{
    if (!a || !b || !c || !d || !M)
        return (0);
    return (1);
}

static void     md5_round1(uint32_t *a, uint32_t *b,
                            uint32_t *c, uint32_t *d, uint32_t *M)
{
    if (!check_rounds(a, b, c, d, M))
        return ;
    MD5_OP(MD5_F(*b,*c,*d), *a,*b,*c,*d, M[0],  T[0],  S[0]);
    MD5_OP(MD5_F(*a,*b,*c), *d,*a,*b,*c, M[1],  T[1],  S[1]);
    MD5_OP(MD5_F(*d,*a,*b), *c,*d,*a,*b, M[2],  T[2],  S[2]);
    MD5_OP(MD5_F(*c,*d,*a), *b,*c,*d,*a, M[3],  T[3],  S[3]);
    MD5_OP(MD5_F(*b,*c,*d), *a,*b,*c,*d, M[4],  T[4],  S[4]);
    MD5_OP(MD5_F(*a,*b,*c), *d,*a,*b,*c, M[5],  T[5],  S[5]);
    MD5_OP(MD5_F(*d,*a,*b), *c,*d,*a,*b, M[6],  T[6],  S[6]);
    MD5_OP(MD5_F(*c,*d,*a), *b,*c,*d,*a, M[7],  T[7],  S[7]);
    MD5_OP(MD5_F(*b,*c,*d), *a,*b,*c,*d, M[8],  T[8],  S[8]);
    MD5_OP(MD5_F(*a,*b,*c), *d,*a,*b,*c, M[9],  T[9],  S[9]);
    MD5_OP(MD5_F(*d,*a,*b), *c,*d,*a,*b, M[10], T[10], S[10]);
    MD5_OP(MD5_F(*c,*d,*a), *b,*c,*d,*a, M[11], T[11], S[11]);
    MD5_OP(MD5_F(*b,*c,*d), *a,*b,*c,*d, M[12], T[12], S[12]);
    MD5_OP(MD5_F(*a,*b,*c), *d,*a,*b,*c, M[13], T[13], S[13]);
    MD5_OP(MD5_F(*d,*a,*b), *c,*d,*a,*b, M[14], T[14], S[14]);
    MD5_OP(MD5_F(*c,*d,*a), *b,*c,*d,*a, M[15], T[15], S[15]);
}

static void     md5_round2(uint32_t *a, uint32_t *b,
                            uint32_t *c, uint32_t *d, uint32_t *M)
{
    if (!check_rounds(a, b, c, d, M))
        return ;
    MD5_OP(MD5_G(*b,*c,*d), *a,*b,*c,*d, M[1],  T[16], S[16]);
    MD5_OP(MD5_G(*a,*b,*c), *d,*a,*b,*c, M[6],  T[17], S[17]);
    MD5_OP(MD5_G(*d,*a,*b), *c,*d,*a,*b, M[11], T[18], S[18]);
    MD5_OP(MD5_G(*c,*d,*a), *b,*c,*d,*a, M[0],  T[19], S[19]);
    MD5_OP(MD5_G(*b,*c,*d), *a,*b,*c,*d, M[5],  T[20], S[20]);
    MD5_OP(MD5_G(*a,*b,*c), *d,*a,*b,*c, M[10], T[21], S[21]);
    MD5_OP(MD5_G(*d,*a,*b), *c,*d,*a,*b, M[15], T[22], S[22]);
    MD5_OP(MD5_G(*c,*d,*a), *b,*c,*d,*a, M[4],  T[23], S[23]);
    MD5_OP(MD5_G(*b,*c,*d), *a,*b,*c,*d, M[9],  T[24], S[24]);
    MD5_OP(MD5_G(*a,*b,*c), *d,*a,*b,*c, M[14], T[25], S[25]);
    MD5_OP(MD5_G(*d,*a,*b), *c,*d,*a,*b, M[3],  T[26], S[26]);
    MD5_OP(MD5_G(*c,*d,*a), *b,*c,*d,*a, M[8],  T[27], S[27]);
    MD5_OP(MD5_G(*b,*c,*d), *a,*b,*c,*d, M[13], T[28], S[28]);
    MD5_OP(MD5_G(*a,*b,*c), *d,*a,*b,*c, M[2],  T[29], S[29]);
    MD5_OP(MD5_G(*d,*a,*b), *c,*d,*a,*b, M[7],  T[30], S[30]);
    MD5_OP(MD5_G(*c,*d,*a), *b,*c,*d,*a, M[12], T[31], S[31]);
}

static void     md5_round3(uint32_t *a, uint32_t *b,
                            uint32_t *c, uint32_t *d, uint32_t *M)
{
    if (!check_rounds(a, b, c, d, M))
        return ;
    MD5_OP(MD5_XOR(*b,*c,*d), *a,*b,*c,*d, M[5],  T[32], S[32]);
    MD5_OP(MD5_XOR(*a,*b,*c), *d,*a,*b,*c, M[8],  T[33], S[33]);
    MD5_OP(MD5_XOR(*d,*a,*b), *c,*d,*a,*b, M[11], T[34], S[34]);
    MD5_OP(MD5_XOR(*c,*d,*a), *b,*c,*d,*a, M[14], T[35], S[35]);
    MD5_OP(MD5_XOR(*b,*c,*d), *a,*b,*c,*d, M[1],  T[36], S[36]);
    MD5_OP(MD5_XOR(*a,*b,*c), *d,*a,*b,*c, M[4],  T[37], S[37]);
    MD5_OP(MD5_XOR(*d,*a,*b), *c,*d,*a,*b, M[7],  T[38], S[38]);
    MD5_OP(MD5_XOR(*c,*d,*a), *b,*c,*d,*a, M[10], T[39], S[39]);
    MD5_OP(MD5_XOR(*b,*c,*d), *a,*b,*c,*d, M[13], T[40], S[40]);
    MD5_OP(MD5_XOR(*a,*b,*c), *d,*a,*b,*c, M[0],  T[41], S[41]);
    MD5_OP(MD5_XOR(*d,*a,*b), *c,*d,*a,*b, M[3],  T[42], S[42]);
    MD5_OP(MD5_XOR(*c,*d,*a), *b,*c,*d,*a, M[6],  T[43], S[43]);
    MD5_OP(MD5_XOR(*b,*c,*d), *a,*b,*c,*d, M[9],  T[44], S[44]);
    MD5_OP(MD5_XOR(*a,*b,*c), *d,*a,*b,*c, M[12], T[45], S[45]);
    MD5_OP(MD5_XOR(*d,*a,*b), *c,*d,*a,*b, M[15], T[46], S[46]);
    MD5_OP(MD5_XOR(*c,*d,*a), *b,*c,*d,*a, M[2],  T[47], S[47]);
}

static void     md5_round4(uint32_t *a, uint32_t *b,
                            uint32_t *c, uint32_t *d, uint32_t *M)
{
    if (!check_rounds(a, b, c, d, M))
        return ;
    MD5_OP(MD5_I(*b,*c,*d), *a,*b,*c,*d, M[0],  T[48], S[48]);
    MD5_OP(MD5_I(*a,*b,*c), *d,*a,*b,*c, M[7],  T[49], S[49]);
    MD5_OP(MD5_I(*d,*a,*b), *c,*d,*a,*b, M[14], T[50], S[50]);
    MD5_OP(MD5_I(*c,*d,*a), *b,*c,*d,*a, M[5],  T[51], S[51]);
    MD5_OP(MD5_I(*b,*c,*d), *a,*b,*c,*d, M[12], T[52], S[52]);
    MD5_OP(MD5_I(*a,*b,*c), *d,*a,*b,*c, M[3],  T[53], S[53]);
    MD5_OP(MD5_I(*d,*a,*b), *c,*d,*a,*b, M[10], T[54], S[54]);
    MD5_OP(MD5_I(*c,*d,*a), *b,*c,*d,*a, M[1],  T[55], S[55]);
    MD5_OP(MD5_I(*b,*c,*d), *a,*b,*c,*d, M[8],  T[56], S[56]);
    MD5_OP(MD5_I(*a,*b,*c), *d,*a,*b,*c, M[15], T[57], S[57]);
    MD5_OP(MD5_I(*d,*a,*b), *c,*d,*a,*b, M[6],  T[58], S[58]);
    MD5_OP(MD5_I(*c,*d,*a), *b,*c,*d,*a, M[13], T[59], S[59]);
    MD5_OP(MD5_I(*b,*c,*d), *a,*b,*c,*d, M[4],  T[60], S[60]);
    MD5_OP(MD5_I(*a,*b,*c), *d,*a,*b,*c, M[11], T[61], S[61]);
    MD5_OP(MD5_I(*d,*a,*b), *c,*d,*a,*b, M[2],  T[62], S[62]);
    MD5_OP(MD5_I(*c,*d,*a), *b,*c,*d,*a, M[9],  T[63], S[63]);
}

void            md5_transform(t_md5_ctx *ctx)
{
    uint32_t    a;
    uint32_t    b;
    uint32_t    c;
    uint32_t    d;

    if (!ctx)
        return ;
    a = MD5_A(ctx);
    b = MD5_B(ctx);
    c = MD5_C(ctx);
    d = MD5_D(ctx);
    md5_round1(&a, &b, &c, &d, ctx->block.words);
    md5_round2(&a, &b, &c, &d, ctx->block.words);
    md5_round3(&a, &b, &c, &d, ctx->block.words);
    md5_round4(&a, &b, &c, &d, ctx->block.words);
    MD5_A(ctx) += a;
    MD5_B(ctx) += b;
    MD5_C(ctx) += c;
    MD5_D(ctx) += d;
}