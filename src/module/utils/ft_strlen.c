#include "utils.h"

static int  find_zero(uint64_t word, int i)
{
    if (i >= 8)
        return (8);
    if (((word >> (i * 8)) & 0xFF) == 0)
        return (i);
    return (find_zero(word, i + 1));
}

static int  count_aligned(const char *s)
{
    uint64_t    word;

    word = *(uint64_t *)s;
    if (HAS_ZERO(word))
        return (find_zero(word, 0));
    return (8 + count_aligned(s + 8));
}

int     ft_strlen(const char *s)
{
    uint64_t    word;
    int         offset;

    offset = (unsigned long)s % 8;
    word = *(uint64_t *)(s - offset);
    word |= (offset ? (1UL << (offset * 8)) - 1 : 0);
    if (HAS_ZERO(word))
        return (find_zero(word, 0) - offset);
    return (8 - offset + count_aligned(s - offset + 8));
}