#ifndef BLOCK_H
# define BLOCK_H

# include <stdint.h>
# include <stddef.h>
# include "input.h"

typedef union   u_block32
{
    uint8_t     bytes[64];
    uint32_t    words[16];
}               t_block32;

typedef union   u_block64
{
    uint8_t     bytes[128];
    uint64_t    words[16];
}               t_block64;

typedef union   u_block_whirlpool
{
    uint8_t     bytes[64];
    uint64_t    words[8];
}               t_block_whirlpool;



#endif