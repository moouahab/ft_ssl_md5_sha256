#include <stdlib.h>
#include "input.h"

void        free_input(t_input *input, bool free_data)
{
    if (!input)
        return ;
    if (free_data && input->data)
        free(input->data);
    free(input);
}