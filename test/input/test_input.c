#include "input.h"
#include "utils.h"
#include "test_utils.h"
#include <fcntl.h>

// ================================================================
// test_input_from_string
// ================================================================

static void     test_string_null(void)
{
    check("NULL str → NULL          ", input_from_string(NULL) == NULL);
}

static void     test_string_empty(void)
{
    t_input     *in;

    in = input_from_string("");
    check("empty → not NULL         ", in != NULL);
    check("empty → size = 0         ", in->size == 0);
    check("empty → data not NULL    ", in->data != NULL);
    free_input(in, false);
}

static void     test_string_abc(void)
{
    t_input     *in;

    in = input_from_string("abc");
    check("abc → not NULL           ", in != NULL);
    check("abc → size = 3           ", in->size == 3);
    check("abc → data[0] = 'a'      ", in->data[0] == 'a');
    check("abc → data[1] = 'b'      ", in->data[1] == 'b');
    check("abc → data[2] = 'c'      ", in->data[2] == 'c');
    free_input(in, false);
}

static void     test_string_long(void)
{
    t_input     *in;
    char        str[128];
    int         i;

    i = 0;
    while (i < 127)
        str[i++] = 'a';
    str[127] = '\0';
    in = input_from_string(str);
    check("127 'a' → not NULL       ", in != NULL);
    check("127 'a' → size = 127     ", in->size == 127);
    check("127 'a' → data[0] = 'a'  ", in->data[0] == 'a');
    check("127 'a' → data[126] = 'a'", in->data[126] == 'a');
    free_input(in, false);
}

static void     test_string_special(void)
{
    t_input     *in;

    in = input_from_string("hello\nworld");
    check("newline → size = 11      ", in != NULL && in->size == 11);
    check("newline → data[5] = \\n  ", in != NULL && in->data[5] == '\n');
    free_input(in, false);

    in = input_from_string("\x00\x01\x02");
    check("null byte → not NULL     ", in != NULL);
    check("null byte → size = 0     ", in != NULL && in->size == 0);
    free_input(in, false);
}

// ================================================================
// test_input_from_fd
// ================================================================

static void     test_fd_invalid(void)
{
    check("fd -1 → NULL             ", input_from_fd(-1) == NULL);
    check("fd -999 → NULL           ", input_from_fd(-999) == NULL);
}

static void     test_fd_file(void)
{
    t_input     *in;
    int         fd;

    fd = open("/tmp/test_input_abc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, "abc", 3);
    close(fd);

    fd = open("/tmp/test_input_abc.txt", O_RDONLY);
    in = input_from_fd(fd);
    close(fd);

    check("file abc → not NULL      ", in != NULL);
    check("file abc → size = 3      ", in != NULL && in->size == 3);
    check("file abc → data[0] = 'a' ", in != NULL && in->data[0] == 'a');
    check("file abc → data[1] = 'b' ", in != NULL && in->data[1] == 'b');
    check("file abc → data[2] = 'c' ", in != NULL && in->data[2] == 'c');
    free_input(in, true);
}

static void     test_fd_empty(void)
{
    t_input     *in;
    int         fd;

    fd = open("/tmp/test_input_empty.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    close(fd);

    fd = open("/tmp/test_input_empty.txt", O_RDONLY);
    in = input_from_fd(fd);
    close(fd);

    check("empty file → not NULL    ", in != NULL);
    check("empty file → size = 0    ", in != NULL && in->size == 0);
    free_input(in, true);
}

static void     test_fd_large(void)
{
    t_input     *in;
    int         fd;
    int         i;
    char        buf[512];

    i = 0;
    while (i < 512)
        buf[i++] = 'x';

    fd = open("/tmp/test_input_large.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, buf, 512);
    close(fd);

    fd = open("/tmp/test_input_large.txt", O_RDONLY);
    in = input_from_fd(fd);
    close(fd);

    check("512 bytes → not NULL     ", in != NULL);
    check("512 bytes → size = 512   ", in != NULL && in->size == 512);
    check("512 bytes → data[0]='x'  ", in != NULL && in->data[0] == 'x');
    check("512 bytes → data[511]='x'", in != NULL && in->data[511] == 'x');
    free_input(in, true);
}

static void     test_fd_exact_64(void)
{
    t_input     *in;
    int         fd;
    char        buf[64];
    int         i;

    i = 0;
    while (i < 64)
        buf[i++] = 'y';

    fd = open("/tmp/test_input_64.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, buf, 64);
    close(fd);

    fd = open("/tmp/test_input_64.txt", O_RDONLY);
    in = input_from_fd(fd);
    close(fd);

    check("64 bytes → not NULL      ", in != NULL);
    check("64 bytes → size = 64     ", in != NULL && in->size == 64);
    check("64 bytes → data[63]='y'  ", in != NULL && in->data[63] == 'y');
    free_input(in, true);
}

static void     test_fd_65(void)
{
    t_input     *in;
    int         fd;
    char        buf[65];
    int         i;

    i = 0;
    while (i < 65)
        buf[i++] = 'z';

    fd = open("/tmp/test_input_65.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, buf, 65);
    close(fd);

    fd = open("/tmp/test_input_65.txt", O_RDONLY);
    in = input_from_fd(fd);
    close(fd);

    check("65 bytes → not NULL      ", in != NULL);
    check("65 bytes → size = 65     ", in != NULL && in->size == 65);
    check("65 bytes → data[64]='z'  ", in != NULL && in->data[64] == 'z');
    free_input(in, true);
}

// ================================================================
// test_free_input
// ================================================================

static void     test_free_null(void)
{
    check("free NULL → no crash     ", (free_input(NULL, false), 1));
    check("free NULL true → no crash", (free_input(NULL, true), 1));
}

static void     test_free_no_data(void)
{
    t_input     *in;

    in = input_from_string("hello");
    check("free false → no crash    ", (free_input(in, false), 1));
}

static void     test_free_with_data(void)
{
    t_input     *in;
    int         fd;

    fd = open("/tmp/test_input_abc.txt", O_RDONLY);
    in = input_from_fd(fd);
    close(fd);
    check("free true → no crash     ", (free_input(in, true), 1));
}

// ================================================================
// main
// ================================================================

int     main(void)
{
    write(1, "=== test_string_null ===\n", 25);
    test_string_null();

    write(1, "=== test_string_empty ===\n", 26);
    test_string_empty();

    write(1, "=== test_string_abc ===\n", 24);
    test_string_abc();

    write(1, "=== test_string_long ===\n", 25);
    test_string_long();

    write(1, "=== test_string_special ===\n", 28);
    test_string_special();

    write(1, "=== test_fd_invalid ===\n", 24);
    test_fd_invalid();

    write(1, "=== test_fd_file ===\n", 21);
    test_fd_file();

    write(1, "=== test_fd_empty ===\n", 22);
    test_fd_empty();

    write(1, "=== test_fd_large ===\n", 22);
    test_fd_large();

    write(1, "=== test_fd_exact_64 ===\n", 25);
    test_fd_exact_64();

    write(1, "=== test_fd_65 ===\n", 19);
    test_fd_65();

    write(1, "=== test_free_null ===\n", 23);
    test_free_null();

    write(1, "=== test_free_no_data ===\n", 26);
    test_free_no_data();

    write(1, "=== test_free_with_data ===\n", 28);
    test_free_with_data();

    return (0);
}