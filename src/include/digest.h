#ifndef DIGEST_H
# define DIGEST_H

# include <stdint.h>
# include <stddef.h>
# include "block.h"


typedef struct  s_md5_ctx
{
    t_block32   block;
    size_t      used;
    uint64_t    msg_size;
    uint32_t    a[4];
}               t_md5_ctx;

typedef struct  s_sha256_ctx
{
    t_block32   block;
    size_t      used;
    uint64_t    msg_size;
    uint32_t    h[8];
}               t_sha256_ctx;

typedef struct  s_sha512_ctx
{
    t_block64   block;
    size_t      used;
    uint64_t    msg_size;
    uint64_t    h[8];
}               t_sha512_ctx;



#endif