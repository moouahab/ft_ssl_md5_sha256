#ifndef INPUT_H
    # define INPUT_H

    # include <stdint.h>
    # include <stdbool.h>
    # include "utils.h"

    typedef struct  s_input
    {
        uint8_t     *data;
        uint64_t    size;
    }               t_input;

    t_input     *input_from_string(char *str);
    t_input     *input_from_fd(int fd);
    t_input     *input_from_stdin(void);
    void        free_input(t_input *input, bool free_data);

#endif