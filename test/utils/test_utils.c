#include "utils.h"
#include "test_utils.h"

static void     print_ok(char *label)
{
    write(1, label, ft_strlen(label));
    write(1, " : OK\n", 6);
}

static void     print_ko(char *label)
{
    write(1, label, ft_strlen(label));
    write(1, " : KO\n", 6);
}

void            check(char *label, bool cond)
{
    if (cond)
        print_ok(label);
    else
        print_ko(label);
}