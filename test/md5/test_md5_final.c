#include "algorithme.h"
#include "md5.h"
#include "test_utils.h"

static void     test_null(void)
{
    check("NULL ctx → NULL      ", md5(NULL) == NULL);
}

static void     test_overflow(void)
{
    t_md5_ctx   ctx = {0};

    md5_init(&ctx);
    ctx.msg_size = 0xFFFFFFFFFFFFFFFF;
    check("overflow → NULL      ", md5(&ctx) == NULL);
}

static void     print_hash(char *label, char *hash)
{
    ft_putstr(label, 1);
    ft_putstr(" : ", 1);
    if (!hash)
        ft_putstr("NULL\n", 1);
    else
    {
        ft_putstr(hash, 1);
        ft_putstr("\n", 1);
    }
}

static void     test_abc(void)
{
    t_md5_ctx   ctx = {0};
    char        *hash;

    md5_init(&ctx);
    ctx.block.bytes[0] = 'a';
    ctx.block.bytes[1] = 'b';
    ctx.block.bytes[2] = 'c';
    ctx.used = 3;
    ctx.msg_size = 3;
    hash = md5(&ctx);
    print_hash("obtenu  ", hash);
    print_hash("attendu ", "900150983cd24fb0d6963f7d28e17f72");
    check("abc → hash correct   ",
        ft_strcmp(hash, "900150983cd24fb0d6963f7d28e17f72") == 0);
    free(hash);
}

static void     test_empty(void)
{
    t_md5_ctx   ctx = {0};
    char        *hash;

    md5_init(&ctx);
    ctx.used = 0;
    ctx.msg_size = 0;
    hash = md5(&ctx);
    print_hash("obtenu  ", hash);
    print_hash("attendu ", "d41d8cd98f00b204e9800998ecf8427e");
    check("empty → hash correct ",
        ft_strcmp(hash, "d41d8cd98f00b204e9800998ecf8427e") == 0);
    free(hash);
}

static void     test_strcmp(void)
{
    char    *s1;
    char    *s2;
    int     i;

    s1 = ft_calloc(33, 1);
    s2 = "900150983cd24fb0d6963f7d28e17f72";
    i = 0;
    while (i < 32)
    {
        s1[i] = s2[i];
        i++;
    }
    s1[32] = '\0';
    ft_putstr("s1 : ", 1);
    ft_putstr(s1, 1);
    ft_putstr("\n", 1);
    ft_putstr("s2 : ", 1);
    ft_putstr(s2, 1);
    ft_putstr("\n", 1);
    ft_putstr("ft_strcmp : ", 1);
    write(1, ft_strcmp(s1, s2) == 0 ? "OK\n" : "KO\n", 3);
    free(s1);
}


static void     test_long(void)
{
    t_md5_ctx   ctx = {0};
    char        *hash;
    int         i;

    md5_init(&ctx);
    i = 0;
    while (i < 55)
    {
        ctx.block.bytes[i] = 'a';
        i++;
    }
    ctx.used = 55;
    ctx.msg_size = 55;
    hash = md5(&ctx);
    check("55 'a' → not NULL    ", hash != NULL);
    check("55 'a' → 32 chars    ", ft_strlen(hash) == 32);
    free(hash);
}

static void     test_strcmp_basic(void)
{
    char    a[] = "abc";
    char    b[] = "abc";
    char    c[] = "abd";

    ft_putstr("abc == abc : ", 1);
    write(1, ft_strcmp(a, b) == 0 ? "OK\n" : "KO\n", 3);

    ft_putstr("abc == abd : ", 1);
    write(1, ft_strcmp(a, c) != 0 ? "OK\n" : "KO\n", 3);

    ft_putstr("abc == \"abc\" : ", 1);
    write(1, ft_strcmp(a, "abc") == 0 ? "OK\n" : "KO\n", 3);

    ft_putstr("calloc == literal : ", 1);
    char *d = ft_calloc(4, 1);
    d[0] = 'a'; d[1] = 'b'; d[2] = 'c';
    write(1, ft_strcmp(d, "abc") == 0 ? "OK\n" : "KO\n", 3);
    free(d);
}


int     main(void)
{
    write(1, "=== test_null ===\n", 18);
    test_null();

    write(1, "=== test_overflow ===\n", 22);
    test_overflow();

    write(1, "=== test_abc ===\n", 17);
    test_abc();

    write(1, "=== test_empty ===\n", 19);
    test_empty();

    write(1, "=== test_long ===\n", 18);
    test_long();

    write(1, "=== test_strcmp ===\n", 20);
    test_strcmp();
    write(1, "=== test_strcmp_basic ===\n", 27);
    test_strcmp_basic();


    return (0);
}