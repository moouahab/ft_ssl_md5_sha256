#include "algorithme.h"
#include "sha512.h"
#include "test_utils.h"

/* ================================================================ */
/* helper                                                            */
/* ================================================================ */

static char     *hash_string(char *str)
{
    t_sha512_ctx    *ctx;
    char            *hash;
    int             i;
    uint64_t        len;

    i = 0;
    while (str[i])
        i++;
    len = (uint64_t)i;

    ctx = (t_sha512_ctx *)ft_calloc(1, sizeof(t_sha512_ctx));
    if (!ctx)
        return (NULL);
    sha512_init(ctx);
    sha512_process(ctx, str, len);
    hash = sha512(ctx);
    free(ctx);
    return (hash);
}

static void     check_hash(char *label, char *input, char *expected)
{
    char    *got;
    int     i;
    int     ok;

    got = hash_string(input);
    if (!got)
    {
        check(label, 0);
        return ;
    }
    ok = 1;
    i = 0;
    while (i < 128)
    {
        if (got[i] != expected[i])
        {
            ok = 0;
            break ;
        }
        i++;
    }
    check(label, ok);
    if (!ok)
    {
        ft_putstr("  attendu : ", 1);
        ft_putstr(expected, 1);
        ft_putstr("\n", 1);
        ft_putstr("  obtenu  : ", 1);
        ft_putstr(got, 1);
        ft_putstr("\n", 1);
    }
    free(got);
}

/* ================================================================ */
/* NULL protection                                                   */
/* ================================================================ */

static void     test_null(void)
{
    check("sha512(NULL) = NULL       ", sha512(NULL) == NULL);
}

/* ================================================================ */
/* vecteurs RFC 6234 + vérifiés contre openssl sha512               */
/* ================================================================ */

static void     test_vectors(void)
{
    check_hash(
        "abc                       ",
        "abc",
        "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea2"
        "0a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd"
        "454d4423643ce80e2a9ac94fa54ca49f"
    );
    check_hash(
        "vide                      ",
        "",
        "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc"
        "83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f"
        "63b931bd47417a81a538327af927da3e"
    );
    check_hash(
        "The quick brown fox       ",
        "The quick brown fox jumps over the lazy dog",
        "07e547d9586f6a73f73fbac0435ed76951218fb7d0c8d788"
        "a309d785436bbb642e93a252a954f23912547d1e8a3b5ed6"
        "e1bfd7097821233fa0538f3db854fee6"
    );
    check_hash(
        "1 char a                  ",
        "a",
        "1f40fc92da241694750979ee6cf582f2d5d7d28e18335de0"
        "5abc54d0560e0f5302860c652bf08d560252aa5e74210546"
        "f369fbbbce8c12cfc7957b2652fe9a75"
    );
    check_hash(
        "111 chars aaaa...         ",
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaa",
        "c0c27aea8dbe169c4cf25176cbf12db708fd6303db8cf94a1"
        "cfb402c1680d3d68f39bc5b9a10970dd5373cb0fe1cb36fa5"
        "0e33165140d72933ba87af9d5d1ffe"
    );

    check_hash(
        "128 chars (bloc plein)    ",
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "b73d1929aa615934e61a871596b3f3b33359f42b8175602e8"
        "9f7e06e5f658a243667807ed300314b95cacdd579f3e33abdf"
        "be351909519a846d465c59582f321"
    );
}

/* ================================================================ */
/* main                                                              */
/* ================================================================ */

int     main(void)
{
    write(1, "=== test_null ===\n", 18);
    test_null();

    write(1, "=== test_vectors ===\n", 21);
    test_vectors();

    return (0);
}