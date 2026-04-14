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
    t_sha256_ctx    ctx = make_ctx();
    uint8_t         buf[4] = {0x61, 0x62, 0x63, 0x00};

    check("NULL ctx → 0              ", sha256_process(NULL, buf, 3) == 0);
    check("NULL data + len=0 → ok    ", sha256_process(&ctx, NULL, 0) == 1);
    check("NULL data + len>0 → 0     ", sha256_process(&ctx, buf, 0) == 1);
}

/* ================================================================ */
/* msg_size                                                          */
/* ================================================================ */

static void     test_msg_size(void)
{
    t_sha256_ctx    ctx = make_ctx();
    uint8_t         buf[64];
    int             i;

    i = 0;
    while (i < 64)
        buf[i++] = 0x61;

    sha256_process(&ctx, buf, 3);
    check("abc → msg_size = 24 bits  ", ctx.msg_size == 24);

    ctx = make_ctx();
    sha256_process(&ctx, buf, 64);
    check("64b → msg_size = 512 bits ", ctx.msg_size == 512);

    ctx = make_ctx();
    sha256_process(&ctx, buf, 55);
    check("55b → msg_size = 440 bits ", ctx.msg_size == 440);
}

/* ================================================================ */
/* used — gestion du bloc courant                                    */
/* ================================================================ */

static void     test_used(void)
{
    t_sha256_ctx    ctx = make_ctx();
    uint8_t         buf[128];
    int             i;

    i = 0;
    while (i < 128)
        buf[i++] = 0x61;

    sha256_process(&ctx, buf, 3);
    check("abc → used = 3            ", ctx.used == 3);

    ctx = make_ctx();
    sha256_process(&ctx, buf, 55);
    check("55b → used = 55           ", ctx.used == 55);

    ctx = make_ctx();
    sha256_process(&ctx, buf, 64);
    check("64b → used = 0 (transform)", ctx.used == 0);

    ctx = make_ctx();
    sha256_process(&ctx, buf, 65);
    check("65b → used = 1            ", ctx.used == 1);

    ctx = make_ctx();
    sha256_process(&ctx, buf, 128);
    check("128b → used = 0           ", ctx.used == 0);

    ctx = make_ctx();
    sha256_process(&ctx, buf, 67);
    check("67b → used = 3            ", ctx.used == 3);
}

/* ================================================================ */
/* accumulation par chunks                                           */
/* ================================================================ */

static void     test_chunks(void)
{
    t_sha256_ctx    ctx_a = make_ctx();
    t_sha256_ctx    ctx_b = make_ctx();
    uint8_t         buf[6] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66};
    int             i;

    sha256_process(&ctx_a, buf, 6);

    sha256_process(&ctx_b, buf,     3);
    sha256_process(&ctx_b, buf + 3, 3);

    check("chunks msg_size egal      ", ctx_a.msg_size == ctx_b.msg_size);
    check("chunks used egal          ", ctx_a.used == ctx_b.used);

    i = 0;
    while (i < (int)ctx_a.used)
    {
        if (ctx_a.block.bytes[i] != ctx_b.block.bytes[i])
            break ;
        i++;
    }
    check("chunks bytes egaux        ", i == (int)ctx_a.used);
}

/* ================================================================ */
/* bloc complet → transform appelé                                   */
/* ================================================================ */

static void     test_transform_called(void)
{
    t_sha256_ctx    ctx_before = make_ctx();
    t_sha256_ctx    ctx_after  = make_ctx();
    uint8_t         buf[64];
    int             i;
    int             changed;

    i = 0;
    while (i < 64)
        buf[i++] = 0x61;

    sha256_process(&ctx_after, buf, 64);

    changed = 0;
    i = 0;
    while (i < 8)
    {
        if (ctx_after.h[i] != ctx_before.h[i])
            changed = 1;
        i++;
    }
    check("64b → H modifies          ", changed == 1);
    check("64b → used = 0 apres      ", ctx_after.used == 0);
}

/* ================================================================ */
/* main                                                              */
/* ================================================================ */

int     main(void)
{
    write(1, "=== test_null ===\n", 18);
    test_null();

    write(1, "=== test_msg_size ===\n", 22);
    test_msg_size();

    write(1, "=== test_used ===\n", 18);
    test_used();

    write(1, "=== test_chunks ===\n", 20);
    test_chunks();

    write(1, "=== test_transform_called ===\n", 30);
    test_transform_called();

    return (0);
}