#ifndef SHA256_H
    # define SHA256_H

        # include "algorithme.h"

        void    sha256_init(t_sha256_ctx *ctx);
        void    sha256_transform(t_sha256_ctx *ctx);
        int     sha256_process(t_sha256_ctx *ctx, const void *data, size_t len);
        int     sha256_padding(t_sha256_ctx *ctx);
        char    *sha256(t_sha256_ctx *ctx);

#endif