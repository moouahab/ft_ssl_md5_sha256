#include "input.h"
#include "utils.h"

t_input     *input_from_fd(int fd)
{
    t_input     *input;
    uint8_t     buffer[64];
    int         bytes_read;
    uint8_t     *new_data;

    if (fd < 0)
        return (NULL);
    input = ft_calloc(1, sizeof(t_input));
    if (!input)
        return (NULL);
    while ((bytes_read = read(fd, buffer, 64)) > 0)
    {
        new_data = ft_realloc(input->data,
                    (int)input->size,
                    (int)(input->size + bytes_read));
        if (!new_data)
            return (free_input(input, true), NULL);
        input->data = new_data;
        copy_bytes(input->data + input->size, buffer, bytes_read);
        input->size += bytes_read;
    }
    if (bytes_read < 0)
        return (free_input(input, true), NULL);
    return (input);
}