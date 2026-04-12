#include "args.h"
#include "test_utils.h"

static void     test_no_args(void)
{
    char    *av[] = {"ft_ssl", NULL};
    t_args  *args;

    args = parse_args(1, av);
    check("no args → NULL        ", args == NULL);
}

static void     test_invalid_command(void)
{
    char    *av[] = {"ft_ssl", "foobar", NULL};
    t_args  *args;

    args = parse_args(2, av);
    check("invalid cmd → NULL    ", args == NULL);
}

static void     test_valid_md5(void)
{
    char    *av[] = {"ft_ssl", "md5", NULL};
    t_args  *args;

    args = parse_args(2, av);
    check("md5 → not NULL        ", args != NULL);
    check("command = md5         ", ft_strcmp(args->command, "md5") == 0);
    check("no flags              ", !args->flags.p && !args->flags.q
                                    && !args->flags.r && !args->flags.s);
    check("no files              ", args->files_count == 0);
    check("no strings            ", args->strings_count == 0);
    ft_free_args(args);
}

static void     test_flags(void)
{
    char    *av[] = {"ft_ssl", "md5", "-p", "-q", "-r", NULL};
    t_args  *args;

    args = parse_args(5, av);
    check("flags → not NULL      ", args != NULL);
    check("flag p                ", args->flags.p == true);
    check("flag q                ", args->flags.q == true);
    check("flag r                ", args->flags.r == true);
    check("flag s = false        ", args->flags.s == false);
    ft_free_args(args);
}

static void     test_string(void)
{
    char    *av[] = {"ft_ssl", "md5", "-s", "hello", NULL};
    t_args  *args;

    args = parse_args(4, av);
    check("string → not NULL     ", args != NULL);
    check("strings_count = 1     ", args->strings_count == 1);
    check("string = hello        ", ft_strcmp(args->strings[0], "hello") == 0);
    ft_free_args(args);
}

static void     test_file(void)
{
    char    *av[] = {"ft_ssl", "sha256", "-r", "file.txt", NULL};
    t_args  *args;

    args = parse_args(4, av);
    check("file → not NULL       ", args != NULL);
    check("files_count = 1       ", args->files_count == 1);
    check("file = file.txt       ", ft_strcmp(args->files[0].name, "file.txt") == 0);
    check("flag r                ", args->flags.r == true);
    ft_free_args(args);
}

static void     test_after_file(void)
{
    char    *av[] = {"ft_ssl", "md5", "-s", "foo", "file", "-s", "bar", NULL};
    t_args  *args;

    args = parse_args(7, av);
    check("after file → not NULL ", args != NULL);
    check("strings_count = 1     ", args->strings_count == 1);
    check("files_count = 3       ", args->files_count == 3);
    check("file[0] = file        ", ft_strcmp(args->files[0].name, "file") == 0);
    check("file[1] = -s          ", ft_strcmp(args->files[1].name, "-s") == 0);
    check("file[2] = bar         ", ft_strcmp(args->files[2].name, "bar") == 0);
    ft_free_args(args);
}

static void     test_multiple_strings(void)
{
    char    *av[] = {"ft_ssl", "md5", "-s", "foo", "-s", "bar", NULL};
    t_args  *args;

    args = parse_args(6, av);
    check("multi str → not NULL  ", args != NULL);
    check("strings_count = 2     ", args->strings_count == 2);
    check("string[0] = foo       ", ft_strcmp(args->strings[0], "foo") == 0);
    check("string[1] = bar       ", ft_strcmp(args->strings[1], "bar") == 0);
    ft_free_args(args);
}

int     main(void)
{
    write(1, "=== test_no_args ===\n", 21);
    test_no_args();
    write(1, "=== test_invalid_command ===\n", 29);
    test_invalid_command();
    write(1, "=== test_valid_md5 ===\n", 23);
    test_valid_md5();
    write(1, "=== test_flags ===\n", 19);
    test_flags();
    write(1, "=== test_string ===\n", 20);
    test_string();
    write(1, "=== test_file ===\n", 18);
    test_file();
    write(1, "=== test_after_file ===\n", 24);
    test_after_file();
    write(1, "=== test_multiple_strings ===\n", 30);
    test_multiple_strings();
    return (0);
}