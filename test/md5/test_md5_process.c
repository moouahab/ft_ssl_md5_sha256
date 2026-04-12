#include "algorithme.h"
#include "md5.h"
#include "test_utils.h"

int     main(void)
{
    t_md5_ctx   ctx = {0};

    // test NULL
    check("NULL protection      ", (md5_process(NULL), 1));

    // test normal — "abc" paddé
    md5_init(&ctx);
    ctx.block.bytes[0] = 'a';
    ctx.block.bytes[1] = 'b';
    ctx.block.bytes[2] = 'c';
    ctx.block.bytes[3] = 0x80;
    ctx.block.words[14] = 24;
    ctx.block.words[15] = 0;
    ctx.used = 3;
    ctx.msg_size = 3;

    md5_process(&ctx);

    // registres corrects
    check("A = 0x98500190       ", MD5_A(&ctx) == 0x98500190);
    check("B = 0xb04fd23c       ", MD5_B(&ctx) == 0xb04fd23c);
    check("C = 0x7d3f96d6       ", MD5_C(&ctx) == 0x7d3f96d6);
    check("D = 0x727fe128       ", MD5_D(&ctx) == 0x727fe128);

    // bloc effacé
    check("used = 0             ", ctx.used == 0);
    check("block.words[0] = 0   ", ctx.block.words[0] == 0);
    check("block.words[14] = 0  ", ctx.block.words[14] == 0);
    check("block.words[15] = 0  ", ctx.block.words[15] == 0);

    return (0);
}