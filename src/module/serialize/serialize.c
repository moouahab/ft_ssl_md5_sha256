#include "algorithme.h"

static bool     check_input(t_input *input, uint64_t max_size)
{
    if (!input || !input->data || !input->size)
        return (false);
    if (input->size > max_size)
        return (false);
    return (true);
}

static bool     check_serialize(t_serial *s, uint64_t size)
{
    if (size <= s->block_size && *s->used != size)
        return (false);
    return (true);
}

t_md5_ctx       *serialize_md5(t_input *input)
{
    t_md5_ctx   *ctx;
    t_serial    s;

    if (!check_input(input, 0x1FFFFFFFFFFFFFFF))
        return (NULL);
    ctx = ft_calloc(1, sizeof(t_md5_ctx));
    if (!ctx)
        return (NULL);
    ctx->msg_size = input->size;
    s.bytes = ctx->block.bytes;
    s.used = &ctx->used;
    s.block_size = 64;
    serialize_bytes(&s, input->data, input->size);
    if (!check_serialize(&s, input->size))
        return (free(ctx), NULL);
    return (ctx);
}

t_sha256_ctx    *serialize_sha256(t_input *input)
{
    t_sha256_ctx    *ctx;
    t_serial        s;

    if (!check_input(input, 0x1FFFFFFFFFFFFFFF))
        return (NULL);
    ctx = ft_calloc(1, sizeof(t_sha256_ctx));
    if (!ctx)
        return (NULL);
    ctx->msg_size = input->size;
    s.bytes = ctx->block.bytes;
    s.used = &ctx->used;
    s.block_size = 64;
    serialize_bytes(&s, input->data, input->size);
    if (!check_serialize(&s, input->size))
        return (free(ctx), NULL);
    return (ctx);
}

t_sha512_ctx    *serialize_sha512(t_input *input)
{
    t_sha512_ctx    *ctx;
    t_serial        s;

    if (!check_input(input, 0x3FFFFFFFFFFFFFFF))
        return (NULL);
    ctx = ft_calloc(1, sizeof(t_sha512_ctx));
    if (!ctx)
        return (NULL);
    ctx->msg_size = input->size;
    s.bytes = ctx->block.bytes;
    s.used = &ctx->used;
    s.block_size = 128;
    serialize_bytes(&s, input->data, input->size);
    if (!check_serialize(&s, input->size))
        return (free(ctx), NULL);
    return (ctx);
}
