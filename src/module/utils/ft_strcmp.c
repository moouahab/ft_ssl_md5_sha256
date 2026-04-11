#include "utils.h"

static int  find_diff(uint64_t w1, uint64_t w2, int byte)
{
    if (byte >= 8)
        return (0);
    if (((w1 >> (byte * 8)) & 0xFF) != ((w2 >> (byte * 8)) & 0xFF))
        return (((w1 >> (byte * 8)) & 0xFF) - ((w2 >> (byte * 8)) & 0xFF));
    return (find_diff(w1, w2, byte + 1));
}

static int  cmp_aligned(const char *s1, const char *s2)
{
    uint64_t    w1;
    uint64_t    w2;

    w1 = *(uint64_t *)s1;
    w2 = *(uint64_t *)s2;
    if (HAS_ZERO(w1) || HAS_ZERO(w2) || w1 != w2)
        return (find_diff(w1, w2, 0));
    return (cmp_aligned(s1 + 8, s2 + 8));
}

static int  cmp_head(const char *s1, const char *s2)
{
    if (*s1 != *s2)
        return ((unsigned char)*s1 - (unsigned char)*s2);
    if (*s1 == '\0')
        return (0);
    if (!((unsigned long)(s1 + 1) % 8) && !((unsigned long)(s2 + 1) % 8))
        return (cmp_aligned(s1 + 1, s2 + 1));
    return (cmp_head(s1 + 1, s2 + 1));
}

int     ft_strcmp(const char *s1, const char *s2)
{
    if (!((unsigned long)s1 % 8) && !((unsigned long)s2 % 8))
        return (cmp_aligned(s1, s2));
    return (cmp_head(s1, s2));
}