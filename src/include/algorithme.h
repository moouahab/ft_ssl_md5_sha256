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

        // SHA256 registres
        # define SHA256_H0(ctx)     (ctx)->h[0]
        # define SHA256_H1(ctx)     (ctx)->h[1]
        # define SHA256_H2(ctx)     (ctx)->h[2]
        # define SHA256_H3(ctx)     (ctx)->h[3]
        # define SHA256_H4(ctx)     (ctx)->h[4]
        # define SHA256_H5(ctx)     (ctx)->h[5]
        # define SHA256_H6(ctx)     (ctx)->h[6]
        # define SHA256_H7(ctx)     (ctx)->h[7]

        // SHA512 registres
        # define SHA512_H0(ctx)     (ctx)->h[0]
        # define SHA512_H1(ctx)     (ctx)->h[1]
        # define SHA512_H2(ctx)     (ctx)->h[2]
        # define SHA512_H3(ctx)     (ctx)->h[3]
        # define SHA512_H4(ctx)     (ctx)->h[4]
        # define SHA512_H5(ctx)     (ctx)->h[5]
        # define SHA512_H6(ctx)     (ctx)->h[6]
        # define SHA512_H7(ctx)     (ctx)->h[7]

#endif