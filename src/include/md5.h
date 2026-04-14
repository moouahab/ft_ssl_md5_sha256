#ifndef MD5_H
# define MD5_H

# include "algorithme.h"

char    *md5(t_md5_ctx *ctx);
void    md5_init(t_md5_ctx *ctx);
void    md5_transform(t_md5_ctx *ctx);
int     md5_process(t_md5_ctx *ctx);
int     md5_padding(t_md5_ctx *ctx);


#endif