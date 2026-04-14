#ifndef UTILS_H
    #define UTILS_H

    # define HAS_ZERO(x) (((x) - 0x0101010101010101UL) & ~(x) & 0x8080808080808080UL)

    #include <stdint.h>
    #include <stddef.h>
    #include <unistd.h>
    #include <stdlib.h>

    typedef struct  s_serial
    {
        uint8_t     *bytes;
        size_t      *used;
        size_t      block_size;
    }               t_serial;


    int     ft_strlen(const char *s);
    int     ft_strcmp(const char *s1, const char *s2);
    void    *ft_calloc(int count, int size);
    void    *ft_realloc(void *ptr, int old_size, int new_size);
    int     ft_putstr(const char *s, int fd);
    void    print_hex(char *label, uint32_t val);
    void    copy_bytes(uint8_t *dst, uint8_t *src, int size);

    // Utils functions for serializing bytes into blocks
    void     serialize_bytes(t_serial *s, uint8_t *src, uint64_t size);




#endif /* UTILS_H */