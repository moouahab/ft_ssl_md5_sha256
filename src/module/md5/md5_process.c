#include "md5.h"

static void     clear_block(t_md5_ctx *ctx, int i)
{
    if (!ctx || i >= 8)
        return ;
    ((uint64_t *)ctx->block.bytes)[i] = 0UL;
    clear_block(ctx, i + 1);
}

int            md5_process(t_md5_ctx *ctx)
{
    if (!ctx)
        return (0);
    md5_transform(ctx);
    clear_block(ctx, 0);
    ctx->used = 0;
    return (1);
}