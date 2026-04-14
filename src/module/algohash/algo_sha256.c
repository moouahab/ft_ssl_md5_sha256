#include "hash.h"
#include "sha256.h"

char    *compute_sha256(t_input *input)
{
    t_sha256_ctx    *ctx;
    char            *hash;

    if (!input)
        return (NULL);
    ctx = ft_calloc(1, sizeof(t_sha256_ctx));
    if (!ctx)
        return (NULL);
    sha256_init(ctx);
    sha256_process(ctx, input->data, input->size);
    hash = sha256(ctx);
    free(ctx);
    return (hash);
}