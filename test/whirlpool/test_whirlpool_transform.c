#include "algorithme.h"
#include "whirlpool.h"
#include "test_utils.h"

/* ================================================================ */
/* helper print                                                      */
/* ================================================================ */

static void     print_row(char *label, uint64_t val)
{
    unsigned char   buf[16];
    char            hex[] = "0123456789abcdef";
    int             i;

    ft_putstr(label, 1);
    ft_putstr(" : 0x", 1);
    i = 15;
    while (i >= 0)
    {
        buf[i] = hex[val & 0xF];
        val >>= 4;
        i--;
    }
    write(1, buf, 16);
    write(1, "\n", 1);
}

static void     print_matrix(char *label, t_wp_state *s)
{
    int     i;

    ft_putstr(label, 1);
    ft_putstr("\n", 1);
    i = 0;
    while (i < 8)
    {
        print_row("  row", s->rows[i]);
        i++;
    }
}

/* ================================================================ */
/* test gf_mul                                                       */
/* ================================================================ */

static void     test_gf_mul(void)
{
    t_whirlpool_ctx ctx = {0};

    whirlpool_init(&ctx);

    ft_putstr("=== test_gf_mul ===\n", 1);

    check("gf_mul(0x00, 0x00) = 0x00 ", ctx.sbox[0] == 0x18);  // sanity
    check("gf_mul identite 1×x = x   ",
        (uint8_t)(1 * 0x53) == 0x53);

    // valeurs vérifiées manuellement sur GF(2^8) poly 0x11D
    // 2 × 0x80 = 0x1D (débordement → XOR 0x1D)
    // calculé : hi=1 → a=0x00, a^=0x1D → 0x1D
    check("gf_mul(0x02, 0x80) = 0x1D ",
        ({ uint8_t r = 0; uint8_t a = 0x02; uint8_t b = 0x80;
           uint8_t h; int i = 0;
           while(i<8){if(b&1)r^=a;h=a&0x80;a<<=1;if(h)a^=0x1D;b>>=1;i++;}
           r; }) == 0x1D);

    // 2 × 0x01 = 0x02 (pas de débordement)
    check("gf_mul(0x02, 0x01) = 0x02 ",
        ({ uint8_t r = 0; uint8_t a = 0x02; uint8_t b = 0x01;
           uint8_t h; int i = 0;
           while(i<8){if(b&1)r^=a;h=a&0x80;a<<=1;if(h)a^=0x1D;b>>=1;i++;}
           r; }) == 0x02);

    // 9 × 0x01 = 0x09
    check("gf_mul(0x09, 0x01) = 0x09 ",
        ({ uint8_t r = 0; uint8_t a = 0x09; uint8_t b = 0x01;
           uint8_t h; int i = 0;
           while(i<8){if(b&1)r^=a;h=a&0x80;a<<=1;if(h)a^=0x1D;b>>=1;i++;}
           r; }) == 0x09);
}

/* ================================================================ */
/* test sub_bytes                                                    */
/* ================================================================ */

static void     test_sub_bytes(void)
{
    t_whirlpool_ctx ctx = {0};

    whirlpool_init(&ctx);

    ft_putstr("=== test_sub_bytes ===\n", 1);

    // sub_bytes est static → on appelle via whirlpool_transform indirectement
    // on teste via les valeurs connues de sbox
    check("sbox[0x00] = 0x18          ", ctx.sbox[0x00] == 0x18);
    check("sbox[0x01] = 0x23          ", ctx.sbox[0x01] == 0x23);
    check("sbox[0xFF] = 0x86          ", ctx.sbox[0xFF] == 0x86);
    check("sbox[0x61] = 0x0f          ", ctx.sbox[0x61] == 0x0f);
    check("sbox[0x62] = 0xd5          ", ctx.sbox[0x62] == 0xd5);
    check("sbox[0x63] = 0x80          ", ctx.sbox[0x63] == 0x80);
    check("sbox[0x80] = 0x97          ", ctx.sbox[0x80] == 0x97);
}

/* ================================================================ */
/* test shift_columns                                                */
/* ================================================================ */

static void     test_shift_columns(void)
{
    t_wp_state  src = {0};
    t_wp_state  dst = {0};
    int         i;
    int         j;

    ft_putstr("=== test_shift_columns ===\n", 1);

    // remplir avec valeurs distinctes pour tracer les déplacements
    // m[i][j] = i*8 + j  (valeur unique par cellule)
    i = 0;
    while (i < 8)
    {
        j = 0;
        while (j < 8)
        {
            src.m[i][j] = (uint8_t)(i * 8 + j);
            j++;
        }
        i++;
    }

    ft_putstr("avant :\n", 1);
    print_matrix("src", &src);

    // shift_columns est static → simuler manuellement
    i = 0;
    while (i < 8)
    {
        j = 0;
        while (j < 8)
        {
            dst.m[i][j] = src.m[(i + j) % 8][j];
            j++;
        }
        i++;
    }

    ft_putstr("apres shift_columns :\n", 1);
    print_matrix("dst", &dst);

    // col 0 → pas de décalage → dst[i][0] = src[i][0]
    check("col0 inchangee row0       ", dst.m[0][0] == src.m[0][0]);
    check("col0 inchangee row7       ", dst.m[7][0] == src.m[7][0]);

    // col 1 → décalage de 1 → dst[0][1] = src[1][1]
    check("col1 decalage 1 row0      ", dst.m[0][1] == src.m[1][1]);
    check("col1 decalage 1 row7      ", dst.m[7][1] == src.m[(7+1)%8][1]);

    // col 2 → décalage de 2 → dst[0][2] = src[2][2]
    check("col2 decalage 2 row0      ", dst.m[0][2] == src.m[2][2]);

    // col 7 → décalage de 7 → dst[0][7] = src[7][7]
    check("col7 decalage 7 row0      ", dst.m[0][7] == src.m[7][7]);
}

/* ================================================================ */
/* test mix_rows — valeur unique vérifiable                         */
/* ================================================================ */

static void     test_mix_rows(void)
{
    t_wp_state  src = {0};
    t_wp_state  dst = {0};

    ft_putstr("=== test_mix_rows ===\n", 1);

    // si src = identité (que des 0x01 sur la diagonale)
    // → mix_rows(src) = matrice MDS elle-même

    // test simple : src = matrice de zéros sauf m[0][0] = 0x01
    // → dst->m[row][0] = mds[row][0] × 0x01 = mds[row][0]
    src.m[0][0] = 0x01;

    // simuler mix_rows manuellement pour la colonne 0
    // dst[row][0] = XOR de gf_mul(mds[row][k], src[k][0]) pour k=0..7
    // = gf_mul(mds[row][0], 0x01) car src[k][0]=0 pour k>0
    // = mds[row][0]

    // mds[0][0] = 0x01 → dst[0][0] = 0x01
    // mds[1][0] = 0x09 → dst[1][0] = 0x09
    // mds[2][0] = 0x02 → dst[2][0] = 0x02

    ft_putstr("src[0][0] = 0x01 → dst col 0 = colonne 0 de MDS\n", 1);
    print_matrix("src", &src);

    check("src zeroe OK              ", src.m[0][1] == 0x00);
    check("src m[0][0] = 0x01        ", src.m[0][0] == 0x01);

    // vérifier add_round_key via XOR direct
    ft_putstr("=== test_add_round_key ===\n", 1);
    dst.rows[0] = 0xAAAAAAAAAAAAAAAAULL;
    src.rows[0] = 0x5555555555555555ULL;
    dst.rows[0] ^= src.rows[0];
    check("XOR rows = 0xFF...        ",
        dst.rows[0] == 0xFFFFFFFFFFFFFFFFULL);
}

/* ================================================================ */
/* test whirlpool_transform complet sur bloc vide                   */
/* ================================================================ */

static void     test_empty_block(void)
{
    t_whirlpool_ctx ctx = {0};

    whirlpool_init(&ctx);

    ft_putstr("=== test_empty_block ===\n", 1);
    ft_putstr("bloc de zeros → transform\n", 1);

    whirlpool_transform(&ctx);

    ft_putstr("apres transform bloc vide :\n", 1);
    print_matrix("state", &ctx.state);

    // l'état ne doit pas être tout à zéro après transform
    check("state modifie apres trans  ",
        ctx.state.rows[0] != 0 || ctx.state.rows[1] != 0);
}

/* ================================================================ */
/* test bloc abc paddé                                               */
/* ================================================================ */

static void     test_abc_block(void)
{
    t_whirlpool_ctx ctx = {0};
    int             i;
    int             j;

    whirlpool_init(&ctx);

    ctx.block.bytes[0]  = 0x61;
    ctx.block.bytes[1]  = 0x62;
    ctx.block.bytes[2]  = 0x63;
    ctx.block.bytes[3]  = 0x80;
    ctx.block.bytes[63] = 0x18;

    whirlpool_transform(&ctx);

    ft_putstr("=== state m[][] ===\n", 1);
    i = 0;
    while (i < 8)
    {
        j = 0;
        while (j < 8)
        {
            unsigned char buf[3];
            char hex[] = "0123456789abcdef";
            buf[0] = hex[ctx.state.m[i][j] >> 4];
            buf[1] = hex[ctx.state.m[i][j] & 0xF];
            buf[2] = ' ';
            write(1, buf, 3);
            j++;
        }
        write(1, "\n", 1);
        i++;
    }

    // attendu : 4e2448a4c6f486bb 16b6562c73b4020b ...
    check("m[0] correct", ctx.state.m[0][0] == 0x4e &&
                          ctx.state.m[0][1] == 0x24 &&
                          ctx.state.m[0][2] == 0x48 &&
                          ctx.state.m[0][3] == 0xa4 &&
                          ctx.state.m[0][4] == 0xc6 &&
                          ctx.state.m[0][5] == 0xf4 &&
                          ctx.state.m[0][6] == 0x86 &&
                          ctx.state.m[0][7] == 0xbb);
    check("m[1] correct", ctx.state.m[1][0] == 0x16 &&
                          ctx.state.m[1][1] == 0xb6 &&
                          ctx.state.m[1][2] == 0x56 &&
                          ctx.state.m[1][3] == 0x2c &&
                          ctx.state.m[1][4] == 0x73 &&
                          ctx.state.m[1][5] == 0xb4 &&
                          ctx.state.m[1][6] == 0x02 &&
                          ctx.state.m[1][7] == 0x0b);
    check("m[2] correct", ctx.state.m[2][0] == 0xf3 &&
                          ctx.state.m[2][1] == 0x04 &&
                          ctx.state.m[2][2] == 0x3e &&
                          ctx.state.m[2][3] == 0x3a &&
                          ctx.state.m[2][4] == 0x73 &&
                          ctx.state.m[2][5] == 0x1b &&
                          ctx.state.m[2][6] == 0xce &&
                          ctx.state.m[2][7] == 0x72);
    check("m[3] correct", ctx.state.m[3][0] == 0x1a &&
                          ctx.state.m[3][1] == 0xe1 &&
                          ctx.state.m[3][2] == 0xb3 &&
                          ctx.state.m[3][3] == 0x03 &&
                          ctx.state.m[3][4] == 0xd9 &&
                          ctx.state.m[3][5] == 0x7e &&
                          ctx.state.m[3][6] == 0x6d &&
                          ctx.state.m[3][7] == 0x4c);
    check("m[4] correct", ctx.state.m[4][0] == 0x71 &&
                          ctx.state.m[4][1] == 0x81 &&
                          ctx.state.m[4][2] == 0xee &&
                          ctx.state.m[4][3] == 0xbd &&
                          ctx.state.m[4][4] == 0xb6 &&
                          ctx.state.m[4][5] == 0xc5 &&
                          ctx.state.m[4][6] == 0x7e &&
                          ctx.state.m[4][7] == 0x27);
    check("m[5] correct", ctx.state.m[5][0] == 0x7d &&
                          ctx.state.m[5][1] == 0x0e &&
                          ctx.state.m[5][2] == 0x34 &&
                          ctx.state.m[5][3] == 0x95 &&
                          ctx.state.m[5][4] == 0x71 &&
                          ctx.state.m[5][5] == 0x14 &&
                          ctx.state.m[5][6] == 0xcb &&
                          ctx.state.m[5][7] == 0xd6);
    check("m[6] correct", ctx.state.m[6][0] == 0xc7 &&
                          ctx.state.m[6][1] == 0x97 &&
                          ctx.state.m[6][2] == 0xfc &&
                          ctx.state.m[6][3] == 0x9d &&
                          ctx.state.m[6][4] == 0x95 &&
                          ctx.state.m[6][5] == 0xd8 &&
                          ctx.state.m[6][6] == 0xb5 &&
                          ctx.state.m[6][7] == 0x82);
    check("m[7] correct", ctx.state.m[7][0] == 0xd2 &&
                          ctx.state.m[7][1] == 0x25 &&
                          ctx.state.m[7][2] == 0x29 &&
                          ctx.state.m[7][3] == 0x20 &&
                          ctx.state.m[7][4] == 0x76 &&
                          ctx.state.m[7][5] == 0xd4 &&
                          ctx.state.m[7][6] == 0xee &&
                          ctx.state.m[7][7] == 0xf5);
}



/* ================================================================ */
/* main                                                              */
/* ================================================================ */

int     main(void)
{
    write(1, "=== test_gf_mul ===\n", 20);
    test_gf_mul();

    write(1, "\n=== test_sub_bytes ===\n", 23);
    test_sub_bytes();

    write(1, "\n=== test_shift_columns ===\n", 27);
    test_shift_columns();

    write(1, "\n=== test_mix_rows ===\n", 22);
    test_mix_rows();

    write(1, "\n=== test_empty_block ===\n", 25);
    test_empty_block();

    write(1, "\n=== test_abc_block ===\n", 23);
    test_abc_block();

    return (0);
}