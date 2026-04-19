#include "algorithme.h"
#include "whirlpool.h"
#include "test_utils.h"

int     main(void)
{
    t_whirlpool_ctx ctx = {0};

    whirlpool_init(&ctx);

    check("NULL protection           ", (whirlpool_init(NULL), 1));
    check("state.rows[0] = 0         ", ctx.state.rows[0] == 0);
    check("state.rows[7] = 0         ", ctx.state.rows[7] == 0);
    check("used = 0                  ", ctx.used == 0);
    check("msg_size = 0              ", ctx.msg_size == 0);

    // sbox[0] = permute(E[0]<<4 | E[0]) = permute(0x11)
    // vérifier quelques valeurs connues de la spec Whirlpool
    check("sbox[0x00] = 0x18         ", ctx.sbox[0x00] == 0x18);
    check("sbox[0x01] = 0x23         ", ctx.sbox[0x01] == 0x23);
    check("sbox[0xFF] = 0x86         ", ctx.sbox[0xFF] == 0x86);

    return (0);
}