#include "algorithme.h"
#include "sha256.h"
#include "input.h"
#include "test_utils.h"

/* ================================================================ */
/* helpers                                                           */
/* ================================================================ */

static char     *hash_string(char *str)
{
    t_sha256_ctx    *ctx;
    char            *hash;
    uint64_t        len;
    int             i;

    i = 0;
    while (str[i])
        i++;
    len = (uint64_t)i;

    ctx = (t_sha256_ctx *)ft_calloc(1, sizeof(t_sha256_ctx));
    if (!ctx)
        return (NULL);
    sha256_init(ctx);
    sha256_process(ctx, str, len);
    hash = sha256(ctx);
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
    while (i < 64)
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
    check("sha256(NULL) = NULL       ", sha256(NULL) == NULL);
}

/* ================================================================ */
/* vecteurs RFC 6234 + vérifiés contre openssl                      */
/* ================================================================ */

static void     test_vectors(void)
{
    check_hash(
        "abc                       ",
        "abc",
        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"
    );
    check_hash(
        "vide                      ",
        "",
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"
    );
    check_hash(
        "abcdbcdecdefdefgefghfghighij",
        "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
        "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1"
    );
    check_hash(
        "The quick brown fox       ",
        "The quick brown fox jumps over the lazy dog",
        "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592"
    );
    check_hash(
        "1 char 'a'                ",
        "a",
        "ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb"
    );
    check_hash(
        "55 chars                  ",
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "9f4390f8d30c2dd92ec9f095b65e2b9ae9b0a925a5258e241c9f1e910f734318"
    );
    check_hash(
        "56 chars (bloc limite)    ",
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "b35439a4ac6f0948b6d6f9e3c6af0f5f590ce20f1bde7090ef7970686ec6738a"
    );
    check_hash(
        "64 chars (bloc plein)     ",
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "ffe054fe7ae0cb6dc65c3af9b61d5209f439851db43d0ba5997337b1b26abb58"
    );
    check_hash(
        "65 chars                  ",
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "635361c48bb9eab14198e76ea8ab7f1a41685d6ad62aa9146d301d4f17eb0ae0"
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