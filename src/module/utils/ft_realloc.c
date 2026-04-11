#include <stdlib.h>
#include "utils.h"

static void     copy_bytes(char *src, char *dst, int size)
{
    if (size <= 0)
        return ;
    *dst = *src;
    copy_bytes(src + 1, dst + 1, size - 1);
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
    copy_bytes((char *)ptr, (char *)new_ptr, copy_size);
    free(ptr);
    return (new_ptr);
}