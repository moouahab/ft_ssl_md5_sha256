#include <unistd.h>
#include "utils.h"
 
int ft_putstr(const char *s, int fd)
{
    if (!s)
        return (write(fd, "(null)", 6));
    return (write(fd, s, ft_strlen(s)));
}

void     print_hex(char *label, uint32_t val)
{
    unsigned char   buf[8];
    char            hex[] = "0123456789abcdef";
    int             i;

    ft_putstr(label, 1);
    ft_putstr(" : 0x", 1);
    i = 7;
    while (i >= 0)
    {
        buf[i] = hex[val & 0xF];
        val >>= 4;
        i--;
    }
    write(1, buf, 8);
    write(1, "\n", 1);
}
