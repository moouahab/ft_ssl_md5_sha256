#include <stdlib.h>
#include "utils.h"

static void     zero_aligned(uint64_t *ptr, int words)
{
    if (words <= 0)
        return ;
    *ptr = 0UL;
    zero_aligned(ptr + 1, words - 1);
}

static void     zero_tail(char *ptr, int bytes)
{
    if (bytes <= 0)
        return ;
    *ptr = 0;
    zero_tail(ptr + 1, bytes - 1);
}

void    *ft_calloc(int count, int size)
{
    void    *ptr;
    int     total;

    if (count <= 0 || size <= 0)
        return (NULL);
    if (count > 0x7FFFFFFF / size)
        return (NULL);
    total = count * size;
    ptr = malloc(total);
    if (!ptr)
        return (NULL);
    zero_aligned((uint64_t *)ptr, total >> 3);
    zero_tail((char *)ptr + (total & ~7), total & 7);
    return (ptr);
}