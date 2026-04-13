#include "hash.h"

static bool     check_disp(t_display *disp)
{
    if (!disp || !disp->hasher)
        return (false);
    if (!disp->hasher->hash || !disp->hasher->name || !disp->hasher->label)
        return (false);
    return (true);
}

static void     write_label(uint8_t *data, uint64_t size, char *buf, int buf_size)
{
    uint64_t    i;
    int         j;

    i = 0;
    j = 0;
    if (size > 0 && data[size - 1] == '\n')
        size--;       
    while (i < size && j < buf_size - 3)
    {
        if (data[i] == '\n')
        {
            buf[j++] = '\\';
            buf[j++] = 'n';
        }
        else if (data[i] == '\t')
        {
            buf[j++] = '\\';
            buf[j++] = 't';
        }
        else
            buf[j++] = (char)data[i];
        i++;
    }
    buf[j] = '\0';
}

static void     print_stdin(t_display *disp, char *hash)
{
    ft_putstr(disp->hasher->label, 1);
    ft_putstr("(stdin)= ", 1);
    ft_putstr(hash, 1);
    ft_putstr("\n", 1);
}

static void     display_stdin_hash(t_display *disp,
                                    char *hash, char *label)
{
    if (disp->quiet)
    {
        if (disp->print_stdin)
        {
            write(1, label, ft_strlen(label));
            write(1, "\n", 1);
        }
        ft_putstr(hash, 1);
        ft_putstr("\n", 1);
        return ;
    }
    if (disp->print_stdin)
    {
        ft_putstr("(\"", 1);
        ft_putstr(label, 1);
        ft_putstr("\")= ", 1);
        ft_putstr(hash, 1);
        ft_putstr("\n", 1);
        return ;
    }
    print_stdin(disp, hash);
}

static void     handle_stdin(t_display *disp)
{
    t_input     *input;
    char        *hash;
    char        label[4096];

    if (!check_disp(disp))
        return ;
    input = input_from_stdin();
    if (!input)
        return ;
    hash = disp->hasher->hash(input);
    if (!hash)
        return (free_input(input, true));
    write_label(input->data, input->size, label, 4096);
    display_stdin_hash(disp, hash, label);
    free(hash);
    free_input(input, true);
}

static void     handle_strings(t_args *args, t_display *disp)
{
    t_input     *input;
    char        *hash;
    int         i;

    if (!args || !check_disp(disp) || args->strings_count < 0)
        return ;
    i = 0;
    while (i < args->strings_count)
    {
        if (args->strings[i])
        {
            input = input_from_string(args->strings[i]);
            if (input)
            {
                hash = disp->hasher->hash(input);
                if (hash)
                {
                    display_hash(disp, hash, args->strings[i], false);
                    free(hash);
                }
                free_input(input, false);
            }
        }
        i++;
    }
}

static void     handle_files(t_args *args, t_display *disp)
{
    t_input     *input;
    char        *hash;
    int         i;

    if (!args || !check_disp(disp) || args->files_count < 0)
        return ;
    i = 0;
    while (i < args->files_count)
    {
        if (!args->files[i].name)
        {
            i++;
            continue ;
        }
        args->files[i].fd = open(args->files[i].name, O_RDONLY);
        if (args->files[i].fd < 0)
        {
            ft_putstr("ft_ssl: ", 2);
            ft_putstr(disp->hasher->name, 2);
            ft_putstr(": ", 2);
            ft_putstr(args->files[i].name, 2);
            ft_putstr(": No such file or directory\n", 2);
            i++;
            continue ;
        }
        input = input_from_fd(args->files[i].fd);
        close(args->files[i].fd);
        args->files[i].fd = -1;
        if (input)
        {
            hash = disp->hasher->hash(input);
            if (hash)
            {
                display_hash(disp, hash, args->files[i].name, true);
                free(hash);
            }
            free_input(input, true);
        }
        i++;
    }
}

void    run_hash(t_args *args, t_hasher *hasher)
{
    t_display   disp;

    if (!args || !hasher)
        return ;
    if (!hasher->hash || !hasher->name || !hasher->label)
        return ;
    disp.hasher       = hasher;
    disp.quiet        = args->flags.q;
    disp.reverse      = args->flags.r;
    disp.print_stdin  = args->flags.p;
    if (disp.print_stdin)
        handle_stdin(&disp);
    handle_strings(args, &disp);
    handle_files(args, &disp);
    // ← si rien traité → lire stdin par défaut
    if (!disp.print_stdin
        && args->strings_count == 0
        && args->files_count == 0)
        handle_stdin(&disp);
}