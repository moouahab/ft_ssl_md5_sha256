#include "algorithme.h"
#include "sha256.h"
#include "test_utils.h"

/* ================================================================ */
/* helpers                                                           */
/* ================================================================ */

static t_sha256_ctx     make_ctx(void)
{
    t_sha256_ctx    ctx;
    int             i;

    i = 0;
    while (i < 16)
    {
        ctx.block.words[i] = 0;
        i++;
    }
    ctx.used     = 0;
    ctx.msg_size = 0;
    sha256_init(&ctx);
    return (ctx);
}

/* ================================================================ */
/* NULL protection                                                   */
/* ================================================================ */

static void     test_null(void)
{
    check("NULL ctx → 0              ", sha256_padding(NULL) == 0);
}

/* ================================================================ */
/* message court (< 56 octets) → un seul bloc                       */
/* ================================================================ */

static void     test_short_message(void)
{
    t_sha256_ctx    ctx = make_ctx();
    uint8_t         buf[3] = {0x61, 0x62, 0x63};

    sha256_process(&ctx, buf, 3);
    ctx.msg_size = 24;
    sha256_padding(&ctx);

    check("court → used = 0 apres    ", ctx.used == 0);
}

/* ================================================================ */
/* message = 55 octets → exactement avant la limite                 */
/* ================================================================ */

static void     test_55_bytes(void)
{
    t_sha256_ctx    ctx = make_ctx();
    uint8_t         buf[55];
    int             i;

    i = 0;
    while (i < 55)
        buf[i++] = 0x61;

    sha256_process(&ctx, buf, 55);
    ctx.msg_size = 440;
    sha256_padding(&ctx);

    check("55b → used = 0 apres      ", ctx.used == 0);
}

/* ================================================================ */
/* message = 56 octets → nécessite un deuxième bloc                 */
/* ================================================================ */

static void     test_56_bytes(void)
{
    t_sha256_ctx    ctx = make_ctx();
    uint8_t         buf[56];
    int             i;

    i = 0;
    while (i < 56)
        buf[i++] = 0x61;

    sha256_process(&ctx, buf, 56);
    ctx.msg_size = 448;
    sha256_padding(&ctx);

    check("56b → used = 0 apres      ", ctx.used == 0);
}

/* ================================================================ */
/* message = 64 octets → bloc plein + padding                       */
/* ================================================================ */

static void     test_64_bytes(void)
{
    t_sha256_ctx    ctx = make_ctx();
    uint8_t         buf[64];
    int             i;

    i = 0;
    while (i < 64)
        buf[i++] = 0x61;

    sha256_process(&ctx, buf, 64);
    ctx.msg_size = 512;
    sha256_padding(&ctx);

    check("64b → used = 0 apres      ", ctx.used == 0);
}

/* ================================================================ */
/* padding → bit '1' correctement placé                             */
/* ================================================================ */

static void     test_padding_bit(void)
{
    t_sha256_ctx    ctx = make_ctx();
    uint8_t         buf[3] = {0x61, 0x62, 0x63};

    sha256_process(&ctx, buf, 3);
    check("avant padding bytes[3]=0  ", ctx.block.bytes[3] == 0x00);
}
/* ================================================================ */
/* retour de sha256_padding                                          */
/* ================================================================ */

static void     test_return_value(void)
{
    t_sha256_ctx    ctx = make_ctx();
    uint8_t         buf[3] = {0x61, 0x62, 0x63};

    sha256_process(&ctx, buf, 3);
    ctx.msg_size = 24;
    check("return 1 sur succes       ", sha256_padding(&ctx) == 1);
    check("return 0 sur NULL         ", sha256_padding(NULL) == 0);
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

    write(1, "=== test_55_bytes ===\n", 22);
    test_55_bytes();

    write(1, "=== test_56_bytes ===\n", 22);
    test_56_bytes();

    write(1, "=== test_64_bytes ===\n", 22);
    test_64_bytes();

    write(1, "=== test_padding_bit ===\n", 25);
    test_padding_bit();

    write(1, "=== test_return_value ===\n", 26);
    test_return_value();

    return (0);
}