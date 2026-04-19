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

typedef union   u_sha512_regs
{
    struct
    {
        uint64_t    a;
        uint64_t    b;
        uint64_t    c;
        uint64_t    d;
        uint64_t    e;
        uint64_t    f;
        uint64_t    g;
        uint64_t    h;
    };
    uint64_t        r[8];
}               t_sha512_regs;

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

typedef union   u_wp_state
{
    uint8_t     bytes[64];
    uint8_t     m[8][8];
    uint64_t    rows[8];
}               t_wp_state;

typedef struct  s_whirlpool_ctx
{
    t_wp_state  state;
    t_wp_state  block;
    uint64_t    used;
    uint64_t    msg_size;
    uint8_t     sbox[256];
    t_wp_state  c[10];
}               t_whirlpool_ctx;



#endif