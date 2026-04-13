#include "hash.h"
#include "input.h"

static void     print_label_file(t_display *disp, char *hash, char *label)
{
    ft_putstr(disp->hasher->label, 1);
    ft_putstr("(", 1);
    ft_putstr(label, 1);
    ft_putstr(")= ", 1);
    ft_putstr(hash, 1);
    ft_putstr("\n", 1);
}

static void     print_label_string(t_display *disp, char *hash, char *label)
{
    ft_putstr(disp->hasher->label, 1);
    ft_putstr("(\"", 1);
    ft_putstr(label, 1);
    ft_putstr("\")= ", 1);
    ft_putstr(hash, 1);
    ft_putstr("\n", 1);
}

static void     print_reverse_file(char *hash, char *label)
{
    ft_putstr(hash, 1);
    ft_putstr(" ", 1);
    ft_putstr(label, 1);
    ft_putstr("\n", 1);
}

static void     print_reverse_string(char *hash, char *label)
{
    ft_putstr(hash, 1);
    ft_putstr(" \"", 1);
    ft_putstr(label, 1);
    ft_putstr("\"\n", 1);
}



void            display_hash(t_display *disp, char *hash,
                             char *label, bool is_file)
{
    if (!disp || !hash || !disp->hasher || !disp->hasher->label)
        return ;
    if (disp->quiet)
    {
        // if (disp->print_stdin)
        // {
        //     write(1, label, ft_strlen(label));
        //     write(1, "\n", 1);
        // }
        ft_putstr(hash, 1);
        ft_putstr("\n", 1);
        return ;
    }
    if (!label)
        return ;
    if (disp->reverse && is_file)
        print_reverse_file(hash, label);
    else if (disp->reverse && !is_file)
        print_reverse_string(hash, label);
    else if (is_file)
        print_label_file(disp, hash, label);
    else
        print_label_string(disp, hash, label);
}