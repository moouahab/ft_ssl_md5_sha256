#include "algorithme.h"
#include "sha256.h"
#include "test_utils.h"

int     main(void)
{
    t_sha256_ctx    ctx = {0};

    sha256_init(&ctx);

    check("NULL protection        ", (sha256_init(NULL), 1));
    check("H0 = 0x6a09e667        ", SHA256_H0(&ctx) == 0x6a09e667);
    check("H1 = 0xbb67ae85        ", SHA256_H1(&ctx) == 0xbb67ae85);
    check("H2 = 0x3c6ef372        ", SHA256_H2(&ctx) == 0x3c6ef372);
    check("H3 = 0xa54ff53a        ", SHA256_H3(&ctx) == 0xa54ff53a);
    check("H4 = 0x510e527f        ", SHA256_H4(&ctx) == 0x510e527f);
    check("H5 = 0x9b05688c        ", SHA256_H5(&ctx) == 0x9b05688c);
    check("H6 = 0x1f83d9ab        ", SHA256_H6(&ctx) == 0x1f83d9ab);
    check("H7 = 0x5be0cd19        ", SHA256_H7(&ctx) == 0x5be0cd19);
    check("used = 0               ", ctx.used == 0);
    check("msg_size = 0           ", ctx.msg_size == 0);
    check("block.words[0] = 0     ", ctx.block.words[0] == 0);

    return (0);
}