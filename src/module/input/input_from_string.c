#include "input.h"
#include "utils.h"

t_input     *input_from_string(char *str)
{
    t_input     *input;

    if (!str)
        return (NULL);
    input = ft_calloc(1, sizeof(t_input));
    if (!input)
        return (NULL);
    input->data = (uint8_t *)str;
    input->size = ft_strlen(str);
    return (input);
}