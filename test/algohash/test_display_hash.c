 #include "hash.h"
#include "test_utils.h"

// ================================================================
// helpers
// ================================================================

static t_hasher     make_hasher(void)
{
    t_hasher    hasher;

    hasher.name  = "md5";
    hasher.label = "MD5";
    hasher.hash  = NULL;
    return (hasher);
}

static t_display    make_display(t_hasher *hasher,
                                bool quiet, bool reverse, bool stdin_flag)
{
    t_display   disp;

    disp.hasher      = hasher;
    disp.quiet       = quiet;
    disp.reverse     = reverse;
    disp.print_stdin = stdin_flag;
    return (disp);
}

// ================================================================
// test protections NULL
// ================================================================

static void     test_null_protections(void)
{
    t_hasher    hasher = make_hasher();
    t_display   disp   = make_display(&hasher, false, false, false);

    check("NULL disp → no crash   ", (display_hash(NULL, "hash", "label", false), 1));
    check("NULL hash → no crash   ", (display_hash(&disp, NULL, "label", false), 1));
    check("NULL all → no crash    ", (display_hash(NULL, NULL, NULL, false), 1));
}

// ================================================================
// test quiet mode
// ================================================================

static void     test_quiet(void)
{
    t_hasher    hasher = make_hasher();
    t_display   disp   = make_display(&hasher, true, false, false);

    write(1, "--- quiet string ---\n", 21);
    display_hash(&disp, "900150983cd24fb0d6963f7d28e17f72", "abc", false);
    write(1, "attendu : 900150983cd24fb0d6963f7d28e17f72\n", 43);

    write(1, "--- quiet file ---\n", 19);
    display_hash(&disp, "900150983cd24fb0d6963f7d28e17f72", "file.txt", true);
    write(1, "attendu : 900150983cd24fb0d6963f7d28e17f72\n", 43);

    write(1, "--- quiet stdin ---\n", 20);
    display_hash(&disp, "900150983cd24fb0d6963f7d28e17f72", NULL, false);
    write(1, "attendu : 900150983cd24fb0d6963f7d28e17f72\n", 43);
}

// ================================================================
// test reverse mode
// ================================================================

static void     test_reverse(void)
{
    t_hasher    hasher = make_hasher();
    t_display   disp   = make_display(&hasher, false, true, false);

    write(1, "--- reverse string ---\n", 23);
    display_hash(&disp, "900150983cd24fb0d6963f7d28e17f72", "abc", false);
    write(1, "attendu : 900150983cd24fb0d6963f7d28e17f72 \"abc\"\n", 49);

    write(1, "--- reverse file ---\n", 21);
    display_hash(&disp, "900150983cd24fb0d6963f7d28e17f72", "file.txt", true);
    write(1, "attendu : 900150983cd24fb0d6963f7d28e17f72 file.txt\n", 52);
}

// ================================================================
// test default mode
// ================================================================

static void     test_default(void)
{
    t_hasher    hasher = make_hasher();
    t_display   disp   = make_display(&hasher, false, false, false);

    write(1, "--- default string ---\n", 23);
    display_hash(&disp, "900150983cd24fb0d6963f7d28e17f72", "abc", false);
    write(1, "attendu : MD5 (\"abc\") = 900150983cd24fb0d6963f7d28e17f72\n", 57);

    write(1, "--- default file ---\n", 21);
    display_hash(&disp, "900150983cd24fb0d6963f7d28e17f72", "file.txt", true);
    write(1, "attendu : MD5 (file.txt) = 900150983cd24fb0d6963f7d28e17f72\n", 61);
}

// ================================================================
// test stdin mode
// ================================================================

static void     test_stdin_display(void)
{
    t_hasher    hasher = make_hasher();
    t_display   disp   = make_display(&hasher, false, false, true);

    write(1, "--- stdin ---\n", 14);
    display_hash(&disp, "900150983cd24fb0d6963f7d28e17f72", NULL, false);
    write(1, "attendu : (\"(stdin)\")= 900150983cd24fb0d6963f7d28e17f72\n", 56);
}

// ================================================================
// test quiet + reverse → quiet gagne
// ================================================================

static void     test_quiet_reverse(void)
{
    t_hasher    hasher = make_hasher();
    t_display   disp   = make_display(&hasher, true, true, false);

    write(1, "--- quiet + reverse ---\n", 24);
    display_hash(&disp, "900150983cd24fb0d6963f7d28e17f72", "abc", false);
    write(1, "attendu : 900150983cd24fb0d6963f7d28e17f72\n", 43);
}

// ================================================================
// main
// ================================================================

int     main(void)
{
    write(1, "=== test_null_protections ===\n", 30);
    test_null_protections();

    write(1, "=== test_quiet ===\n", 19);
    test_quiet();

    write(1, "=== test_reverse ===\n", 21);
    test_reverse();

    write(1, "=== test_default ===\n", 21);
    test_default();

    write(1, "=== test_stdin_display ===\n", 27);
    test_stdin_display();

    write(1, "=== test_quiet_reverse ===\n", 27);
    test_quiet_reverse();

    return (0);
}