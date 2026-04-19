#include "algorithme.h"
#include "whirlpool.h"
#include "test_utils.h"

int main(void)
{
    t_whirlpool_ctx ctx = {0};
    int i, j;

    whirlpool_init(&ctx);
    
    // "abc"
    whirlpool_process(&ctx, "abc", 3);
    
    ft_putstr("=== AVANT padding ===\n", 1);
    ft_putstr("used = ", 1);
    print_hex("", ctx.used);
    ft_putstr("\nmsg_size = ", 1);
    print_hex("", ctx.msg_size);
    ft_putstr(" (bits)\n", 1);
    
    ft_putstr("bloc avant padding :\n", 1);
    i = 0;
    while (i < 8)
    {
        j = 0;
        while (j < 8)
        {
            unsigned char buf[3];
            char hex[] = "0123456789abcdef";
            buf[0] = hex[ctx.block.m[i][j] >> 4];
            buf[1] = hex[ctx.block.m[i][j] & 0xF];
            buf[2] = ' ';
            write(1, buf, 3);
            j++;
        }
        write(1, "\n", 1);
        i++;
    }
    
    whirlpool_padding(&ctx);
    
    ft_putstr("\n=== APRES padding ===\n", 1);
    ft_putstr("used = ", 1);
    print_hex("", ctx.used);
    ft_putstr("\n", 1);
    
    ft_putstr("bloc apres padding (devrait etre vide car transform) :\n", 1);
    i = 0;
    while (i < 8)
    {
        j = 0;
        while (j < 8)
        {
            unsigned char buf[3];
            char hex[] = "0123456789abcdef";
            buf[0] = hex[ctx.block.m[i][j] >> 4];
            buf[1] = hex[ctx.block.m[i][j] & 0xF];
            buf[2] = ' ';
            write(1, buf, 3);
            j++;
        }
        write(1, "\n", 1);
        i++;
    }
    
    ft_putstr("\nstate final :\n", 1);
    i = 0;
    while (i < 8)
    {
        j = 0;
        while (j < 8)
        {
            unsigned char buf[3];
            char hex[] = "0123456789abcdef";
            buf[0] = hex[ctx.state.m[i][j] >> 4];
            buf[1] = hex[ctx.state.m[i][j] & 0xF];
            buf[2] = ' ';
            write(1, buf, 3);
            j++;
        }
        write(1, "\n", 1);
        i++;
    }
    
    ft_putstr("\nattendu (openssl) :\n", 1);
    ft_putstr("4e 24 48 a4 c6 f4 86 bb\n", 1);
    ft_putstr("16 b6 56 2c 73 b4 02 0b\n", 1);
    ft_putstr("f3 04 3e 3a 73 1b ce 72\n", 1);
    ft_putstr("1a e1 b3 03 d9 7e 6d 4c\n", 1);
    ft_putstr("71 81 ee bd b6 c5 7e 27\n", 1);
    ft_putstr("7d 0e 34 95 71 14 cb d6\n", 1);
    ft_putstr("c7 97 fc 9d 95 d8 b5 82\n", 1);
    ft_putstr("d2 25 29 20 76 d4 ee f5\n", 1);
    
    return 0;
}