#include <stdlib.h>
#include "utils.h"

void    copy_bytes(uint8_t *dst, uint8_t *src, int size)
{
    if (!dst || !src || size <= 0)
        return ;
    if (size >= 8 && !((unsigned long)dst % 8) && !((unsigned long)src % 8))
    {
        *(uint64_t *)dst = *(uint64_t *)src;
        copy_bytes(dst + 8, src + 8, size - 8);
        return ;
    }
    *dst = *src;
    copy_bytes(dst + 1, src + 1, size - 1);
}

void    *ft_realloc(void *ptr, int old_size, int new_size)
{
    void    *new_ptr;
    int     copy_size;

    if (new_size <= 0)
        return (free(ptr), NULL);
    if (!ptr)
        return (ft_calloc(1, new_size));
    if (new_size == old_size)
        return (ptr);
    new_ptr = malloc(new_size);
    if (!new_ptr)
        return (free(ptr), NULL);
    copy_size = old_size < new_size ? old_size : new_size;
    copy_bytes((uint8_t *)new_ptr, (uint8_t *)ptr, copy_size);
    free(ptr);
    return (new_ptr);
}