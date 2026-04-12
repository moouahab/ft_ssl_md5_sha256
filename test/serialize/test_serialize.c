#include "serialize.h"
#include "test_utils.h"

static void     test_null_input(void)
{
    check("NULL input → NULL     ", serialize_md5(NULL) == NULL);
    check("NULL sha256 → NULL    ", serialize_sha256(NULL) == NULL);
    check("NULL sha512 → NULL    ", serialize_sha512(NULL) == NULL);
}

static void     test_null_data(void)
{
    t_input     input;

    input.data = NULL;
    input.size = 3;
    check("NULL data md5 → NULL  ", serialize_md5(&input) == NULL);
}

static void     test_zero_size(void)
{
    t_input     input;

    input.data = (uint8_t *)"abc";
    input.size = 0;
    check("size=0 md5 → NULL     ", serialize_md5(&input) == NULL);
}

static void     test_overflow(void)
{
    t_input     input;

    input.data = (uint8_t *)"abc";
    input.size = 0xFFFFFFFFFFFFFFFF;
    check("overflow md5 → NULL   ", serialize_md5(&input) == NULL);
}

static void     test_md5_abc(void)
{
    t_input     input;
    t_md5_ctx   *ctx;

    input.data = (uint8_t *)"abc";
    input.size = 3;
    ctx = serialize_md5(&input);
    check("md5 abc → not NULL    ", ctx != NULL);
    check("bytes[0] = 'a'        ", ctx->block.bytes[0] == 'a');
    check("bytes[1] = 'b'        ", ctx->block.bytes[1] == 'b');
    check("bytes[2] = 'c'        ", ctx->block.bytes[2] == 'c');
    check("used = 3              ", ctx->used == 3);
    check("msg_size = 3          ", ctx->msg_size == 3);
    check("bytes[3] = 0          ", ctx->block.bytes[3] == 0);
    free(ctx);
}

static void     test_words_union(void)
{
    t_input     input;
    t_md5_ctx   *ctx;

    input.data = (uint8_t *)"abcd";
    input.size = 4;
    ctx = serialize_md5(&input);
    check("words[0] little-endian",
        ctx->block.words[0] == (uint32_t)('a' | 'b' << 8 | 'c' << 16 | 'd' << 24));
    check("words[1] = 0          ", ctx->block.words[1] == 0);
    free(ctx);
}

static void     test_sha256(void)
{
    t_input         input;
    t_sha256_ctx    *ctx;

    input.data = (uint8_t *)"hello";
    input.size = 5;
    ctx = serialize_sha256(&input);
    check("sha256 → not NULL     ", ctx != NULL);
    check("used = 5              ", ctx->used == 5);
    check("msg_size = 5          ", ctx->msg_size == 5);
    free(ctx);
}

static void     test_sha512(void)
{
    t_input         input;
    t_sha512_ctx    *ctx;

    input.data = (uint8_t *)"hello";
    input.size = 5;
    ctx = serialize_sha512(&input);
    check("sha512 → not NULL     ", ctx != NULL);
    check("used = 5              ", ctx->used == 5);
    check("msg_size = 5          ", ctx->msg_size == 5);
    free(ctx);
}

int     main(void)
{
    write(1, "=== test_null_input ===\n", 24);
    test_null_input();
    write(1, "=== test_null_data ===\n", 23);
    test_null_data();
    write(1, "=== test_zero_size ===\n", 23);
    test_zero_size();
    write(1, "=== test_overflow ===\n", 22);
    test_overflow();
    write(1, "=== test_md5_abc ===\n", 21);
    test_md5_abc();
    write(1, "=== test_words_union ===\n", 25);
    test_words_union();
    write(1, "=== test_sha256 ===\n", 20);
    test_sha256();
    write(1, "=== test_sha512 ===\n", 20);
    test_sha512();
    return (0);
}