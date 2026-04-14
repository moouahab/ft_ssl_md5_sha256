# Documentation — SHA-256 Init

---

## Rôle

`sha256_init` initialise le contexte SHA-256 avec les valeurs de départ
définies par la RFC 6234. Ces valeurs sont les 32 premiers bits de la
partie fractionnaire des racines carrées des 8 premiers nombres premiers.

---

## Valeurs initiales

```
√2  → H0 = 0x6a09e667
√3  → H1 = 0xbb67ae85
√5  → H2 = 0x3c6ef372
√7  → H3 = 0xa54ff53a
√11 → H4 = 0x510e527f
√13 → H5 = 0x9b05688c
√17 → H6 = 0x1f83d9ab
√19 → H7 = 0x5be0cd19
```

---

## Implémentation

```c
void    sha256_init(t_sha256_ctx *ctx)
{
    int     i;

    if (!ctx)
        return ;
    i = 0;
    while (i < 16)
    {
        ctx->block.words[i] = 0;
        i++;
    }
    ctx->used     = 0;
    ctx->msg_size = 0;
    SHA256_H0(ctx) = 0x6a09e667;
    SHA256_H1(ctx) = 0xbb67ae85;
    SHA256_H2(ctx) = 0x3c6ef372;
    SHA256_H3(ctx) = 0xa54ff53a;
    SHA256_H4(ctx) = 0x510e527f;
    SHA256_H5(ctx) = 0x9b05688c;
    SHA256_H6(ctx) = 0x1f83d9ab;
    SHA256_H7(ctx) = 0x5be0cd19;
}
```

---

## Différence avec MD5

| | MD5 | SHA-256 |
|---|---|---|
| Registres | 4 (A B C D) | 8 (H0..H7) |
| Taille hash | 128 bits | 256 bits |
| Endianness | little-endian | big-endian |
| Valeurs init | RFC 1321 | RFC 6234 |

---

## Macros d'accès

```c
# define SHA256_H0(ctx)  (ctx)->h[0]
# define SHA256_H1(ctx)  (ctx)->h[1]
# define SHA256_H2(ctx)  (ctx)->h[2]
# define SHA256_H3(ctx)  (ctx)->h[3]
# define SHA256_H4(ctx)  (ctx)->h[4]
# define SHA256_H5(ctx)  (ctx)->h[5]
# define SHA256_H6(ctx)  (ctx)->h[6]
# define SHA256_H7(ctx)  (ctx)->h[7]
```

---

## Fichiers

```
src/module/sha256/sha256_init.c
src/include/sha256.h
src/include/algorithme.h  → macros SHA256_H0..H7
```