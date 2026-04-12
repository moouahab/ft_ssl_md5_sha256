#include "md5.h"
#include "test_utils.h"

int     main(void)
{
    t_md5_ctx   ctx;

    md5_init(&ctx);

    check("A = 0x67452301", MD5_A(&ctx) == 0x67452301);
    check("B = 0xEFCDAB89", MD5_B(&ctx) == 0xEFCDAB89);
    check("C = 0x98BADCFE", MD5_C(&ctx) == 0x98BADCFE);
    check("D = 0x10325476", MD5_D(&ctx) == 0x10325476);
    check("NULL protection", (md5_init(NULL), 1));

    return (0);
}