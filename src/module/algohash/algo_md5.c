#include "hash.h"
#include "serialize.h"
#include "md5.h"

 char    *compute_md5(t_input *input)
{
    t_md5_ctx   *ctx;
    char        *hash;

    if (!input)
        return (NULL);
    ctx = ft_calloc(1, sizeof(t_md5_ctx));
    if (!ctx)
        return (NULL);
    md5_init(ctx);
    ctx->msg_size = input->size;
    serialize_bytes(&(t_serial){ctx->block.bytes, &ctx->used, 64},
                    input->data, input->size);
    hash = md5(ctx);
    free(ctx);
    return (hash);
}