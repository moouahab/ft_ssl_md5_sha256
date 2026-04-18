#include "algorithme.h"
#include "sha512.h"
#include "test_utils.h"

/* ================================================================ */
/* helper print 64 bits                                              */
/* ================================================================ */

static void     print_x64(char *label, uint64_t val)
{
    unsigned char   buf[16];
    char            hex[] = "0123456789abcdef";
    int             i;

    ft_putstr(label, 1);
    ft_putstr(" : 0x", 1);
    i = 15;
    while (i >= 0)
    {
        buf[i] = hex[val & 0xF];
        val >>= 4;
        i--;
    }
    write(1, buf, 16);
    write(1, "\n", 1);
}

/* ================================================================ */
/* NULL protection                                                   */
/* ================================================================ */

static void     test_null(void)
{
    check("NULL ctx → no crash       ", (sha512_transform(NULL), 1));
}

/* ================================================================ */
/* test schedule W[0] et W[15]                                      */
/* "abc" paddé :                                                     */
/* bytes[0..2] = 0x61 0x62 0x63                                     */
/* bytes[3]    = 0x80                                               */
/* bytes[119..127] = taille 24 bits big-endian                      */
/* ================================================================ */

static void     test_schedule(void)
{
    t_sha512_ctx    ctx = {0};

    sha512_init(&ctx);
    ctx.block.bytes[0] = 0x61;
    ctx.block.bytes[1] = 0x62;
    ctx.block.bytes[2] = 0x63;
    ctx.block.bytes[3] = 0x80;
    ctx.block.bytes[127] = 0x18;

    ft_putstr("=== schedule ===\n", 1);
    print_x64("W[0]  avant swap ", ctx.block.words[0]);
    print_x64("W[0]  apres swap ", SHA512_SWAP(ctx.block.words[0]));
    print_x64("W[15] avant swap ", ctx.block.words[15]);
    print_x64("W[15] apres swap ", SHA512_SWAP(ctx.block.words[15]));

    check("W[0] swap = 0x6162638000000000",
        SHA512_SWAP(ctx.block.words[0]) == 0x6162638000000000ULL);
    check("W[15] swap = 0x0000000000000018",
        SHA512_SWAP(ctx.block.words[15]) == 0x0000000000000018ULL);
}

/* ================================================================ */
/* test hash complet "abc"                                           */
/* valeur attendue vérifiée contre openssl sha512                   */
/* ================================================================ */

static void     test_abc(void)
{
    t_sha512_ctx    ctx = {0};

    sha512_init(&ctx);

    /* bloc paddé de "abc" :
       bytes[0..2]  = 'a' 'b' 'c'
       bytes[3]     = 0x80
       bytes[127]   = 0x18  (24 bits en big-endian sur 128 octets)
    */
    ctx.block.bytes[0]   = 0x61;
    ctx.block.bytes[1]   = 0x62;
    ctx.block.bytes[2]   = 0x63;
    ctx.block.bytes[3]   = 0x80;
    ctx.block.bytes[127] = 0x18;
    ctx.used     = 3;
    ctx.msg_size = 24;

    sha512_transform(&ctx);

    ft_putstr("=== apres transform ===\n", 1);
    print_x64("H0", SHA512_H0(&ctx));
    print_x64("H1", SHA512_H1(&ctx));
    print_x64("H2", SHA512_H2(&ctx));
    print_x64("H3", SHA512_H3(&ctx));
    print_x64("H4", SHA512_H4(&ctx));
    print_x64("H5", SHA512_H5(&ctx));
    print_x64("H6", SHA512_H6(&ctx));
    print_x64("H7", SHA512_H7(&ctx));

    ft_putstr("=== attendu (openssl sha512 'abc') ===\n", 1);
    ft_putstr("ddaf35a193617aba cc417349ae204131\n", 1);
    ft_putstr("12e6fa4e89a97ea2 0a9eeee64b55d39a\n", 1);
    ft_putstr("2192992a274fc1a8 36ba3c23a3feebbd\n", 1);
    ft_putstr("454d4423643ce80e 2a9ac94fa54ca49f\n", 1);

    check("H0 = 0xddaf35a193617aba",
        SHA512_H0(&ctx) == 0xddaf35a193617abaULL);
    check("H1 = 0xcc417349ae204131",
        SHA512_H1(&ctx) == 0xcc417349ae204131ULL);
    check("H2 = 0x12e6fa4e89a97ea2",
        SHA512_H2(&ctx) == 0x12e6fa4e89a97ea2ULL);
    check("H3 = 0x0a9eeee64b55d39a",
        SHA512_H3(&ctx) == 0x0a9eeee64b55d39aULL);
    check("H4 = 0x2192992a274fc1a8",
        SHA512_H4(&ctx) == 0x2192992a274fc1a8ULL);
    check("H5 = 0x36ba3c23a3feebbd",
        SHA512_H5(&ctx) == 0x36ba3c23a3feebbdULL);
    check("H6 = 0x454d4423643ce80e",
        SHA512_H6(&ctx) == 0x454d4423643ce80eULL);
    check("H7 = 0x2a9ac94fa54ca49f",
        SHA512_H7(&ctx) == 0x2a9ac94fa54ca49fULL);
}

/* ================================================================ */
/* main                                                              */
/* ================================================================ */

int     main(void)
{
    write(1, "=== test_null ===\n", 18);
    test_null();

    write(1, "=== test_schedule ===\n", 22);
    test_schedule();

    write(1, "=== test_abc ===\n", 17);
    test_abc();

    return (0);
}