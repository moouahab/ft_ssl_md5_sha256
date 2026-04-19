#include "algorithme.h"
#include "whirlpool.h"
#include "test_utils.h"

static t_whirlpool_ctx  make_ctx(void)
{
    t_whirlpool_ctx ctx;
    int             i;

    i = 0;
    while (i < 8)
    {
        ctx.block.rows[i] = 0;
        ctx.state.rows[i] = 0;
        i++;
    }
    ctx.used     = 0;
    ctx.msg_size = 0;
    whirlpool_init(&ctx);
    return (ctx);
}

/* ================================================================ */
/* NULL protection                                                   */
/* ================================================================ */

static void     test_null(void)
{
    t_whirlpool_ctx ctx = make_ctx();
    uint8_t         buf[4] = {0x61, 0x62, 0x63, 0x00};

    check("NULL ctx → 0              ", whirlpool_process(NULL, buf, 3) == 0);
    check("NULL data + len=0 → ok    ", whirlpool_process(&ctx, NULL, 0) == 1);
    check("NULL data + len>0 → 0     ", whirlpool_process(&ctx, buf, 0) == 1);
}

/* ================================================================ */
/* msg_size                                                          */
/* ================================================================ */

static void     test_msg_size(void)
{
    t_whirlpool_ctx ctx = make_ctx();
    uint8_t         buf[128];
    int             i;

    i = 0;
    while (i < 128)
        buf[i++] = 0x61;

    whirlpool_process(&ctx, buf, 3);
    check("abc → msg_size = 24 bits  ", ctx.msg_size == 24);

    ctx = make_ctx();
    whirlpool_process(&ctx, buf, 64);
    check("64b → msg_size = 512 bits ", ctx.msg_size == 512);

    ctx = make_ctx();
    whirlpool_process(&ctx, buf, 31);
    check("31b → msg_size = 248 bits ", ctx.msg_size == 248);
}

/* ================================================================ */
/* used — gestion du bloc courant (64 octets)                       */
/* ================================================================ */

static void     test_used(void)
{
    t_whirlpool_ctx ctx = make_ctx();
    uint8_t         buf[128];
    int             i;

    i = 0;
    while (i < 128)
        buf[i++] = 0x61;

    whirlpool_process(&ctx, buf, 3);
    check("abc → used = 3            ", ctx.used == 3);

    ctx = make_ctx();
    whirlpool_process(&ctx, buf, 31);
    check("31b → used = 31           ", ctx.used == 31);

    ctx = make_ctx();
    whirlpool_process(&ctx, buf, 64);
    check("64b → used = 0 (transform)", ctx.used == 0);

    ctx = make_ctx();
    whirlpool_process(&ctx, buf, 65);
    check("65b → used = 1            ", ctx.used == 1);

    ctx = make_ctx();
    whirlpool_process(&ctx, buf, 128);
    check("128b → used = 0           ", ctx.used == 0);

    ctx = make_ctx();
    whirlpool_process(&ctx, buf, 67);
    check("67b → used = 3            ", ctx.used == 3);
}

/* ================================================================ */
/* accumulation par chunks                                           */
/* ================================================================ */

static void     test_chunks(void)
{
    t_whirlpool_ctx ctx_a = make_ctx();
    t_whirlpool_ctx ctx_b = make_ctx();
    uint8_t         buf[6] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66};
    int             i;

    whirlpool_process(&ctx_a, buf, 6);

    whirlpool_process(&ctx_b, buf,     3);
    whirlpool_process(&ctx_b, buf + 3, 3);

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
    t_whirlpool_ctx ctx_before = make_ctx();
    t_whirlpool_ctx ctx_after  = make_ctx();
    uint8_t         buf[64];
    int             i;
    int             changed;

    i = 0;
    while (i < 64)
        buf[i++] = 0x61;

    whirlpool_process(&ctx_after, buf, 64);

    changed = 0;
    i = 0;
    while (i < 8)
    {
        if (ctx_after.state.rows[i] != ctx_before.state.rows[i])
            changed = 1;
        i++;
    }
    check("64b → state modifie       ", changed == 1);
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

    write(1, "=== test_transform_called ===\n", 31);
    test_transform_called();

    return (0);
}