#include "whirlpool.h"

static uint8_t  gf_mul(uint8_t a, uint8_t b)
{
    uint8_t result;
    uint8_t hi;
    int     i;

    result = 0;
    i = 0;
    while (i < 8)
    {
        if (b & 1)
            result ^= a;
        hi = a & 0x80;
        a <<= 1;
        if (hi)
            a ^= 0x1D;
        b >>= 1;
        i++;
    }
    return (result);
}

static void     sub_bytes(t_wp_state *state, uint8_t *sbox)
{
    int     i;
    int     j;

    i = 0;
    while (i < 8)
    {
        j = 0;
        while (j < 8)
        {
            state->m[i][j] = sbox[state->m[i][j]];
            j++;
        }
        i++;
    }
}

static void     shift_columns(t_wp_state *dst, t_wp_state *src)
{
    int     col;
    int     row;

    col = 0;
    while (col < 8)
    {
        row = 0;
        while (row < 8)
        {
            dst->m[row][col] = src->m[(row - col + 8) % 8][col];
            row++;
        }
        col++;
    }
}

static void     mix_rows(t_wp_state *dst, t_wp_state *src)
{
    static const uint8_t    mds[8][8] = {
        {0x01, 0x01, 0x04, 0x01, 0x08, 0x05, 0x02, 0x09},
        {0x09, 0x01, 0x01, 0x04, 0x01, 0x08, 0x05, 0x02},
        {0x02, 0x09, 0x01, 0x01, 0x04, 0x01, 0x08, 0x05},
        {0x05, 0x02, 0x09, 0x01, 0x01, 0x04, 0x01, 0x08},
        {0x08, 0x05, 0x02, 0x09, 0x01, 0x01, 0x04, 0x01},
        {0x01, 0x08, 0x05, 0x02, 0x09, 0x01, 0x01, 0x04},
        {0x04, 0x01, 0x08, 0x05, 0x02, 0x09, 0x01, 0x01},
        {0x01, 0x04, 0x01, 0x08, 0x05, 0x02, 0x09, 0x01}
    };
    int     row;
    int     col;
    int     k;
    uint8_t acc;

    row = 0;
    while (row < 8)
    {
        col = 0;
        while (col < 8)
        {
            acc = 0;
            k = 0;
            while (k < 8)
            {
                acc ^= gf_mul(src->m[row][k], mds[k][col]);
                k++;
            }
            dst->m[row][col] = acc;
            col++;
        }
        row++;
    }
}

static void     add_round_key(t_wp_state *state, t_wp_state *key)
{
    int     i;

    i = 0;
    while (i < 8)
    {
        state->rows[i] ^= key->rows[i];
        i++;
    }
}

static void     copy_state(t_wp_state *dst, t_wp_state *src)
{
    int     i;

    i = 0;
    while (i < 8)
    {
        dst->rows[i] = src->rows[i];
        i++;
    }
}

static void     wp_round(t_wp_state *state, t_wp_state *key,
                          uint8_t *sbox, t_wp_state *c)
{
    t_wp_state  tmp_key;
    t_wp_state  tmp_state;
    t_wp_state  new_key;

    // key schedule
    copy_state(&tmp_key, key);
    sub_bytes(&tmp_key, sbox);
    shift_columns(&new_key, &tmp_key);
    mix_rows(key, &new_key);
    add_round_key(key, c);

    // state
    sub_bytes(state, sbox);
    shift_columns(&tmp_state, state);
    mix_rows(state, &tmp_state);
    add_round_key(state, key);
}

// PROBLÈME POTENTIEL: L'ordre des bytes dans le bloc
// Whirlpool utilise big-endian et traite le message byte par byte
void    whirlpool_transform(t_whirlpool_ctx *ctx)
{
    t_wp_state  state;
    t_wp_state  key;
    t_wp_state  block;
    int         r;
    int         i;
    int         j;

    if (!ctx)
        return ;
    
    // Copier le state courant dans key
    copy_state(&key, &ctx->state);
    
    // Convertir le bloc en matrice 8x8
    // IMPORTANT: Whirlpool lit les bytes dans l'ordre
    i = 0;
    while (i < 8)
    {
        j = 0;
        while (j < 8)
        {
            block.m[i][j] = ctx->block.bytes[i * 8 + j];
            j++;
        }
        i++;
    }
    
    // Initialiser state avec block XOR key
    copy_state(&state, &block);
    add_round_key(&state, &key);
    
    // 10 rounds
    r = 0;
    while (r < 10)
    {
        wp_round(&state, &key, ctx->sbox, &ctx->c[r]);
        r++;
    }
    
    // Finalisation: state XOR state_initial XOR block
    i = 0;
    while (i < 8)
    {
        j = 0;
        while (j < 8)
        {
            ctx->state.m[i][j] = state.m[i][j] 
                                ^ ctx->state.m[i][j] 
                                ^ block.m[i][j];
            j++;
        }
        i++;
    }
}