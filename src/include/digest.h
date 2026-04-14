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

typedef union   u_sha256_regs
{
    struct
    {
        uint32_t    a;
        uint32_t    b;
        uint32_t    c;
        uint32_t    d;
        uint32_t    e;
        uint32_t    f;
        uint32_t    g;
        uint32_t    h;
    };
    uint32_t        r[8];
}               t_sha256_regs;

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