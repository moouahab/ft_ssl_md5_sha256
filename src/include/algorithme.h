#ifndef ALGORITHME_H
    # define ALGORITHME_H

        # include <stdint.h>
        # include <stddef.h>
        # include <stdbool.h>
        # include "utils.h"
        # include "input.h"
        # include "block.h"
        # include "digest.h"

        // MD5 registres
        # define MD5_A(ctx)         (ctx)->a[0]
        # define MD5_B(ctx)         (ctx)->a[1]
        # define MD5_C(ctx)         (ctx)->a[2]
        # define MD5_D(ctx)         (ctx)->a[3]

        // MD5 fonctions logiques
        # define MD5_F(b, c, d)     (((b) & (c)) | (~(b) & (d)))
        # define MD5_G(b, c, d)     (((b) & (d)) | ((c) & ~(d)))
        # define MD5_XOR(b, c, d)   ((b) ^ (c) ^ (d))
        # define MD5_I(b, c, d)     ((c) ^ ((b) | ~(d)))

        // MD5 rotation et opération
        # define MD5_ROTATE(x, n)   (((x) << (n)) | ((x) >> (32 - (n))))
        # define MD5_OP(f, a, b, c, d, m, t, s) \
            (a) = (b) + MD5_ROTATE((a) + (f) + (m) + (t), (s))

        // ================================================================
        // SHA-256 registres
        // ================================================================
        # define SHA256_H0(ctx)     (ctx)->h[0]
        # define SHA256_H1(ctx)     (ctx)->h[1]
        # define SHA256_H2(ctx)     (ctx)->h[2]
        # define SHA256_H3(ctx)     (ctx)->h[3]
        # define SHA256_H4(ctx)     (ctx)->h[4]
        # define SHA256_H5(ctx)     (ctx)->h[5]
        # define SHA256_H6(ctx)     (ctx)->h[6]
        # define SHA256_H7(ctx)     (ctx)->h[7]
        // ================================================================
        // rotation droite
        // ================================================================
        # define SHA256_ROTR(x, n)  (((x) >> (n)) | ((x) << (32 - (n))))
        // ================================================================
        // sigma schedule
        // ================================================================
        # define SHA256_s0(x)  (SHA256_ROTR(x,7)  ^ SHA256_ROTR(x,18) ^ ((x) >> 3))
        # define SHA256_s1(x)  (SHA256_ROTR(x,17) ^ SHA256_ROTR(x,19) ^ ((x) >> 10))
        // ================================================================
        // Sigma rounds
        // ================================================================
        # define SHA256_S0(x)  (SHA256_ROTR(x,2)  ^ SHA256_ROTR(x,13) ^ SHA256_ROTR(x,22))
        # define SHA256_S1(x)  (SHA256_ROTR(x,6)  ^ SHA256_ROTR(x,11) ^ SHA256_ROTR(x,25))
        // ================================================================
        // fonctions logiques
        // ================================================================
        # define SHA256_CH(e,f,g)   (((e) & (f)) ^ (~(e) & (g)))
        # define SHA256_MAJ(a,b,c)  (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)))
        // ================================================================
        // swap big-endian
        // ================================================================
        # define SHA256_SWAP(x) \
            (((x) >> 24) | (((x) >> 8) & 0x0000FF00) | \
            (((x) << 8) & 0x00FF0000) | ((x) << 24))

        // ================================================================
        // SHA-512 registres
        // ================================================================
        # define SHA512_H0(ctx)     (ctx)->h[0]
        # define SHA512_H1(ctx)     (ctx)->h[1]
        # define SHA512_H2(ctx)     (ctx)->h[2]
        # define SHA512_H3(ctx)     (ctx)->h[3]
        # define SHA512_H4(ctx)     (ctx)->h[4]
        # define SHA512_H5(ctx)     (ctx)->h[5]
        # define SHA512_H6(ctx)     (ctx)->h[6]
        # define SHA512_H7(ctx)     (ctx)->h[7]

        // ================================================================
        // SHA-512 opérations
        // ================================================================
        // manquant dans algorithme.h
        # define SHA512_ROTR(x, n)  (((x) >> (n)) | ((x) << (64 - (n))))
        # define SHA512_s0(x)  (SHA512_ROTR(x,1)  ^ SHA512_ROTR(x,8)  ^ ((x) >> 7))
        # define SHA512_s1(x)  (SHA512_ROTR(x,19) ^ SHA512_ROTR(x,61) ^ ((x) >> 6))
        # define SHA512_S0(x)  (SHA512_ROTR(x,28) ^ SHA512_ROTR(x,34) ^ SHA512_ROTR(x,39))
        # define SHA512_S1(x)  (SHA512_ROTR(x,14) ^ SHA512_ROTR(x,18) ^ SHA512_ROTR(x,41))
        # define SHA512_CH(e,f,g)   (((e) & (f)) ^ (~(e) & (g)))
        # define SHA512_MAJ(a,b,c)  (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)))
        # define SHA512_SWAP(x) \
            (((x) >> 56) | (((x) >> 40) & 0xFF00ULL) | \
            (((x) >> 24) & 0xFF0000ULL) | (((x) >> 8) & 0xFF000000ULL) | \
            (((x) << 8) & 0xFF00000000ULL) | (((x) << 24) & 0xFF0000000000ULL) | \
            (((x) << 40) & 0xFF000000000000ULL) | ((x) << 56))

#endif