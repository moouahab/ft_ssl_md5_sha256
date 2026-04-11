#include "utils.h"

void     serialize_bytes(t_serial *s, uint8_t *src, uint64_t size)
{
    uint64_t    space;
    uint64_t    i;

    if (!s || !src || !size)
        return ;
    space = s->block_size - *s->used;
    if (size > space)
        size = space;
    i = 0;
    while (i + 8 <= size && !(*s->used % 8))
    {
        *(uint64_t *)(s->bytes + *s->used) = *(uint64_t *)(src + i);
        *s->used += 8;
        i += 8;
    }
    while (i < size)
    {
        s->bytes[*s->used] = src[i];
        (*s->used)++;
        i++;
    }
}
