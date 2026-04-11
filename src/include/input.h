#ifndef INPUT_H
# define INPUT_H

# include <stdint.h>
# include <stddef.h>


typedef struct  s_input
{
    uint8_t     *data;
    uint64_t    size;
}               t_input;


#endif