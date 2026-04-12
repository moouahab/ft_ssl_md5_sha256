#include "algorithme.h"
#include "md5.h"
#include "test_utils.h"

int     main(void)
{
    t_md5_ctx   ctx;

    md5_init(&ctx);

    
    ctx.block.bytes[0] = 'a';
    ctx.block.bytes[1] = 'b';
    ctx.block.bytes[2] = 'c';
    ctx.block.bytes[3] = 0x80;
    ctx.block.words[14] = 24;
    ctx.block.words[15] = 0;
    ctx.used = 3;
    ctx.msg_size = 3;

    // valeurs initiales
    ft_putstr("=== init ===\n", 1);
    print_hex("A", MD5_A(&ctx));
    print_hex("B", MD5_B(&ctx));
    print_hex("C", MD5_C(&ctx));
    print_hex("D", MD5_D(&ctx));

    // verifier M[0]
    ft_putstr("=== bloc ===\n", 1);
    print_hex("M[0]  ", ctx.block.words[0]);
    print_hex("M[14] ", ctx.block.words[14]);

    md5_transform(&ctx);

    ft_putstr("=== apres transform ===\n", 1);
    print_hex("A", MD5_A(&ctx));
    print_hex("B", MD5_B(&ctx));
    print_hex("C", MD5_C(&ctx));
    print_hex("D", MD5_D(&ctx));

    ft_putstr("=== attendu ===\n", 1);
    ft_putstr("A : 0x98500190\n", 1);
    ft_putstr("B : 0xb04fd23c\n", 1);
    ft_putstr("C : 0x7d3f96d6\n", 1);
    ft_putstr("D : 0x727fe128\n", 1);

    return (0);
}