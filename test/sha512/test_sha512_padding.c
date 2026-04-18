#include "algorithme.h"
#include "sha512.h"
#include "test_utils.h"

static t_sha512_ctx     make_ctx(void)
{
    t_sha512_ctx    ctx;
    int             i;

    i = 0;
    while (i < 16)
    {
        ctx.block.words[i] = 0;
        i++;
    }
    ctx.used     = 0;
    ctx.msg_size = 0;
    sha512_init(&ctx);
    return (ctx);
}

/* ================================================================ */
/* NULL protection                                                   */
/* ================================================================ */

static void     test_null(void)
{
    check("NULL ctx → 0              ", sha512_padding(NULL) == 0);
}

/* ================================================================ */
/* message court (< 112 octets) → un seul bloc                      */
/* ================================================================ */

static void     test_short_message(void)
{
    t_sha512_ctx    ctx = make_ctx();
    uint8_t         buf[3] = {0x61, 0x62, 0x63};

    sha512_process(&ctx, buf, 3);
    ctx.msg_size = 24;
    sha512_padding(&ctx);

    check("court → used = 0 apres    ", ctx.used == 0);
}

/* ================================================================ */
/* message = 111 octets → exactement avant la limite                */
/* ================================================================ */

static void     test_111_bytes(void)
{
    t_sha512_ctx    ctx = make_ctx();
    uint8_t         buf[111];
    int             i;

    i = 0;
    while (i < 111)
        buf[i++] = 0x61;

    sha512_process(&ctx, buf, 111);
    ctx.msg_size = 888;
    sha512_padding(&ctx);

    check("111b → used = 0 apres     ", ctx.used == 0);
}

/* ================================================================ */
/* message = 112 octets → nécessite un deuxième bloc                */
/* ================================================================ */

static void     test_112_bytes(void)
{
    t_sha512_ctx    ctx = make_ctx();
    uint8_t         buf[112];
    int             i;

    i = 0;
    while (i < 112)
        buf[i++] = 0x61;

    sha512_process(&ctx, buf, 112);
    ctx.msg_size = 896;
    sha512_padding(&ctx);

    check("112b → used = 0 apres     ", ctx.used == 0);
}

/* ================================================================ */
/* message = 128 octets → bloc plein + padding                      */
/* ================================================================ */

static void     test_128_bytes(void)
{
    t_sha512_ctx    ctx = make_ctx();
    uint8_t         buf[128];
    int             i;

    i = 0;
    while (i < 128)
        buf[i++] = 0x61;

    sha512_process(&ctx, buf, 128);
    ctx.msg_size = 1024;
    sha512_padding(&ctx);

    check("128b → used = 0 apres     ", ctx.used == 0);
}

/* ================================================================ */
/* padding → bit '1' correctement placé                             */
/* ================================================================ */

static void     test_padding_bit(void)
{
    t_sha512_ctx    ctx = make_ctx();
    uint8_t         buf[3] = {0x61, 0x62, 0x63};

    sha512_process(&ctx, buf, 3);
    check("avant padding bytes[3]=0  ", ctx.block.bytes[3] == 0x00);
}

/* ================================================================ */
/* retour de sha512_padding                                          */
/* ================================================================ */

static void     test_return_value(void)
{
    t_sha512_ctx    ctx = make_ctx();
    uint8_t         buf[3] = {0x61, 0x62, 0x63};

    sha512_process(&ctx, buf, 3);
    ctx.msg_size = 24;
    check("return 1 sur succes       ", sha512_padding(&ctx) == 1);
    check("return 0 sur NULL         ", sha512_padding(NULL) == 0);
}

/* ================================================================ */
/* main                                                              */
/* ================================================================ */

int     main(void)
{
    write(1, "=== test_null ===\n", 18);
    test_null();

    write(1, "=== test_short_message ===\n", 27);
    test_short_message();

    write(1, "=== test_111_bytes ===\n", 23);
    test_111_bytes();

    write(1, "=== test_112_bytes ===\n", 23);
    test_112_bytes();

    write(1, "=== test_128_bytes ===\n", 23);
    test_128_bytes();

    write(1, "=== test_padding_bit ===\n", 25);
    test_padding_bit();

    write(1, "=== test_return_value ===\n", 26);
    test_return_value();

    return (0);
}