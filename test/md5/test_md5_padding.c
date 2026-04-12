#include "algorithme.h"
#include "md5.h"
#include "test_utils.h"

int     main(void)
{
    t_md5_ctx   ctx = {0};

    // test NULL
    check("NULL protection      ", (md5_padding(NULL), 1));

    // cas 1 — used = 3 → tient dans le bloc
    md5_init(&ctx);
    ctx.used = 3;
    ctx.msg_size = 3;
    ctx.block.bytes[0] = 'a';
    ctx.block.bytes[1] = 'b';
    ctx.block.bytes[2] = 'c';
    md5_padding(&ctx);
    check("0x80 en [3]          ", ctx.block.bytes[3] == 0x80);
    check("0x00 en [4]          ", ctx.block.bytes[4] == 0x00);
    check("0x00 en [55]         ", ctx.block.bytes[55] == 0x00);
    check("taille bits [14]     ", ctx.block.words[14] == 24);
    check("taille bits [15]     ", ctx.block.words[15] == 0);

    // cas 2 — used = 55 → limite exacte
    md5_init(&ctx);
    ctx.used = 55;
    ctx.msg_size = 55;
    md5_padding(&ctx);
    check("0x80 en [55] used=55 ", ctx.block.bytes[55] == 0x80);
    check("taille = 440 bits    ", ctx.block.words[14] == 440);

    // cas 3 — used = 56 → deuxième bloc nécessaire
    md5_init(&ctx);
    ctx.used = 56;
    ctx.msg_size = 56;
    md5_padding(&ctx);
    check("used=0 apres process ", ctx.used == 0);
    check("taille = 448 bits    ", ctx.block.words[14] == 448);

    // cas 4 — used = 63 → deuxième bloc
    md5_init(&ctx);
    ctx.used = 63;
    ctx.msg_size = 63;
    md5_padding(&ctx);
    check("used=0 apres process ", ctx.used == 0);
    check("taille = 504 bits    ", ctx.block.words[14] == 504);

    return (0);
}