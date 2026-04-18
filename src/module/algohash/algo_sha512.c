#include "hash.h"
#include "sha512.h"

char    *compute_sha512(t_input *input)
{
    t_sha512_ctx    *ctx;
    char            *hash;

    if (!input)
        return (NULL);
    ctx = ft_calloc(1, sizeof(t_sha512_ctx));
    if (!ctx)
        return (NULL);
    sha512_init(ctx);
    sha512_process(ctx, input->data, input->size);
    hash = sha512(ctx);
    free(ctx);
    return (hash);
}