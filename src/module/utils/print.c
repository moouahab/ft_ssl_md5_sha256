#include <unistd.h>
#include "utils.h"
 
int ft_putstr(const char *s, int fd)
{
    if (!s)
        return (write(fd, "(null)", 6));
    return (write(fd, s, ft_strlen(s)));
}