#include "algorithme.h"
#include "sha512.h"
#include "test_utils.h"

int     main(void)
{
    t_sha512_ctx    ctx = {0};

    sha512_init(&ctx);

    check("NULL protection           ", (sha512_init(NULL), 1));
    check("H0 = 0x6a09e667f3bcc908  ", SHA512_H0(&ctx) == 0x6a09e667f3bcc908ULL);
    check("H1 = 0xbb67ae8584caa73b  ", SHA512_H1(&ctx) == 0xbb67ae8584caa73bULL);
    check("H2 = 0x3c6ef372fe94f82b  ", SHA512_H2(&ctx) == 0x3c6ef372fe94f82bULL);
    check("H3 = 0xa54ff53a5f1d36f1  ", SHA512_H3(&ctx) == 0xa54ff53a5f1d36f1ULL);
    check("H4 = 0x510e527fade682d1  ", SHA512_H4(&ctx) == 0x510e527fade682d1ULL);
    check("H5 = 0x9b05688c2b3e6c1f  ", SHA512_H5(&ctx) == 0x9b05688c2b3e6c1fULL);
    check("H6 = 0x1f83d9abfb41bd6b  ", SHA512_H6(&ctx) == 0x1f83d9abfb41bd6bULL);
    check("H7 = 0x5be0cd19137e2179  ", SHA512_H7(&ctx) == 0x5be0cd19137e2179ULL);
    check("used = 0                  ", ctx.used == 0);
    check("msg_size = 0              ", ctx.msg_size == 0);
    check("block.words[0] = 0        ", ctx.block.words[0] == 0);

    return (0);
}