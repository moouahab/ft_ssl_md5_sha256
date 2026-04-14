#include "input.h"

t_input     *input_from_stdin(void)
{
    return (input_from_fd(0));
}