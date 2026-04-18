#ifndef SHA512_H
# define SHA512_H

# include "algorithme.h"

void    sha512_init(t_sha512_ctx *ctx);
void    sha512_transform(t_sha512_ctx *ctx);
int     sha512_process(t_sha512_ctx *ctx, const void *data, uint64_t len);
int     sha512_padding(t_sha512_ctx *ctx);
char    *sha512(t_sha512_ctx *ctx);

#endif