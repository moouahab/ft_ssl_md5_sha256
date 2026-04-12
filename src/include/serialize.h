#ifndef SERIALIZE_H
    # define SERIALIZE_H

        # include "algorithme.h"

        t_md5_ctx       *serialize_md5(t_input *input);
        t_sha256_ctx    *serialize_sha256(t_input *input);
        t_sha512_ctx    *serialize_sha512(t_input *input);

#endif