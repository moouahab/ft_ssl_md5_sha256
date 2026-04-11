#ifndef ALGORITHME_H
    # define ALGORITHME_H

    # include <stdlib.h>
    # include "utils.h"
    # include "input.h"
    # include "block.h"
    # include "digest.h"

    // MD5
    # define MD5_A(ctx)     (ctx)->a[0]
    # define MD5_B(ctx)     (ctx)->a[1]
    # define MD5_C(ctx)     (ctx)->a[2]
    # define MD5_D(ctx)     (ctx)->a[3]

    // SHA256
    # define SHA256_H0(ctx) (ctx)->h[0]
    # define SHA256_H1(ctx) (ctx)->h[1]
    # define SHA256_H2(ctx) (ctx)->h[2]
    # define SHA256_H3(ctx) (ctx)->h[3]
    # define SHA256_H4(ctx) (ctx)->h[4]
    # define SHA256_H5(ctx) (ctx)->h[5]
    # define SHA256_H6(ctx) (ctx)->h[6]
    # define SHA256_H7(ctx) (ctx)->h[7]

    // SHA512
    # define SHA512_H0(ctx) (ctx)->h[0]
    # define SHA512_H1(ctx) (ctx)->h[1]
    # define SHA512_H2(ctx) (ctx)->h[2]
    # define SHA512_H3(ctx) (ctx)->h[3]
    # define SHA512_H4(ctx) (ctx)->h[4]
    # define SHA512_H5(ctx) (ctx)->h[5]
    # define SHA512_H6(ctx) (ctx)->h[6]
    # define SHA512_H7(ctx) (ctx)->h[7]

    t_md5_ctx       *serialize_md5(t_input *input);
    t_sha256_ctx    *serialize_sha256(t_input *input);
    t_sha512_ctx    *serialize_sha512(t_input *input);  


#endif