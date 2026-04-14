#include "algorithme.h"
#include "sha256.h"
#include "test_utils.h"
#include <unistd.h>

static void     print_x(char *label, uint32_t val)
{
    unsigned char   buf[8];
    char            hex[] = "0123456789abcdef";
    int             i;

    ft_putstr(label, 1);
    ft_putstr(" : 0x", 1);
    i = 7;
    while (i >= 0)
    {
        buf[i] = hex[val & 0xF];
        val >>= 4;
        i--;
    }
    write(1, buf, 8);
    write(1, "\n", 1);
}

static void     test_null(void)
{
    check("NULL protection         ", (sha256_transform(NULL), 1));
}

static void     test_schedule(void)
{
    t_sha256_ctx    ctx = {0};
    uint32_t        W[16];
    int             i;

    sha256_init(&ctx);
    
    // "abc" + padding 0x80
    ctx.block.bytes[0] = 0x61; 
    ctx.block.bytes[1] = 0x62;
    ctx.block.bytes[2] = 0x63;
    ctx.block.bytes[3] = 0x80;
    ctx.block.bytes[63] = 0x18;

    i = 0;
    while (i < 16)
    {
        W[i] = SHA256_SWAP(ctx.block.words[i]);
        i++;
    }

    ft_putstr("=== schedule ===\n", 1);
    print_x("W[0] ", W[0]);
    print_x("W[15]", W[15]);

    check("W[0]  = 0x61626380      ", W[0]  == 0x61626380);
    check("W[15] = 0x00000018      ", W[15] == 0x00000018);
}

static void     test_abc(void)
{
    t_sha256_ctx    ctx = {0};

    sha256_init(&ctx);

    // Préparation manuelle identique au standard pour "abc"
    ctx.block.bytes[0] = 0x61; 
    ctx.block.bytes[1] = 0x62; 
    ctx.block.bytes[2] = 0x63; 
    ctx.block.bytes[3] = 0x80; 
    ctx.block.bytes[63] = 0x18; 
    
    ctx.used     = 3;
    ctx.msg_size = 3;

    ft_putstr("=== H avant transform ===\n", 1);
    print_x("H0", SHA256_H0(&ctx));

    sha256_transform(&ctx);

    ft_putstr("=== apres transform ===\n", 1);
    print_x("H0", SHA256_H0(&ctx));
    print_x("H1", SHA256_H1(&ctx));
    print_x("H2", SHA256_H2(&ctx));
    print_x("H3", SHA256_H3(&ctx));
    print_x("H4", SHA256_H4(&ctx));
    print_x("H5", SHA256_H5(&ctx));
    print_x("H6", SHA256_H6(&ctx));
    print_x("H7", SHA256_H7(&ctx));

    ft_putstr("=== attendu (Officiel) ===\n", 1);
    ft_putstr("ba7816bf 8f01cfea 414140de 5dae2223\n", 1);
    ft_putstr("b00361a3 96177a9c b410ff61 f20015ad\n", 1);

    // Ces checks vont maintenant passer car ils utilisent les vraies valeurs SHA-256
    check("H0 = 0xba7816bf ", SHA256_H0(&ctx) == 0xba7816bf);
    check("H1 = 0x8f01cfea ", SHA256_H1(&ctx) == 0x8f01cfea);
    check("H2 = 0x414140de ", SHA256_H2(&ctx) == 0x414140de);
    check("H3 = 0x5dae2223 ", SHA256_H3(&ctx) == 0x5dae2223);
    check("H4 = 0xb00361a3 ", SHA256_H4(&ctx) == 0xb00361a3);
    check("H5 = 0x96177a9c ", SHA256_H5(&ctx) == 0x96177a9c);
    check("H6 = 0xb410ff61 ", SHA256_H6(&ctx) == 0xb410ff61);
    check("H7 = 0xf20015ad ", SHA256_H7(&ctx) == 0xf20015ad);
}

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