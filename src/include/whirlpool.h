#ifndef WHIRLPOOL_H
# define WHIRLPOOL_H

# include "algorithme.h"
# include "digest.h"

void    whirlpool_init(t_whirlpool_ctx *ctx);
void    whirlpool_transform(t_whirlpool_ctx *ctx);
int     whirlpool_process(t_whirlpool_ctx *ctx, const void *data, uint64_t len);
int     whirlpool_padding(t_whirlpool_ctx *ctx);
char    *whirlpool(t_whirlpool_ctx *ctx);

#endif