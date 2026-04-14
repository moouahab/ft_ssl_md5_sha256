# Documentation — SHA-256 Transform

---

## Rôle

`sha256_transform` traite un bloc de 512 bits (64 octets) et met à jour
les 8 registres H0..H7. C'est le cœur de l'algorithme SHA-256.

---

## Vue d'ensemble

```
bloc 64 octets
    ↓
build_schedule → W[0..63]   (64 mots 32 bits)
    ↓
init registres → r = {H0..H7}
    ↓
64 rounds → sha256_round × 64
    ↓
addition finale → H[i] += r[i]
```

---

## `t_sha256_regs` — union des registres de travail

```c
typedef union   u_sha256_regs
{
    struct
    {
        uint32_t    a;
        uint32_t    b;
        uint32_t    c;
        uint32_t    d;
        uint32_t    e;
        uint32_t    f;
        uint32_t    g;
        uint32_t    h;
    };
    uint32_t        r[8];
}               t_sha256_regs;
```

L'union permet deux modes d'accès :

```c
// accès par nom → lisible dans sha256_round
regs.a  regs.b  regs.c  regs.d
regs.e  regs.f  regs.g  regs.h

// accès par indice → pratique pour init et addition finale
regs.r[0] .. regs.r[7]
```

---

## `build_schedule` — construction du message schedule

SHA-256 étend les 16 mots du bloc en 64 mots :

```c
W[i] = SHA256_SWAP(M[i])                              // i = 0..15
W[i] = σ1(W[i-2]) + W[i-7] + σ0(W[i-15]) + W[i-16]  // i = 16..63
```

### Pourquoi SHA256_SWAP

SHA-256 est big-endian. La machine est little-endian.
Les octets du bloc sont en mémoire little-endian → il faut les inverser
pour obtenir les mots dans l'ordre big-endian attendu par l'algorithme.

```c
// exemple pour "abc" paddé
M[0] en mémoire  = 0x80636261  (little-endian)
SHA256_SWAP(M[0]) = 0x61626380  ← "a"=0x61 "b"=0x62 "c"=0x63 0x80
```

### Macros sigma (schedule)

```c
# define SHA256_s0(x)  (ROTR(x,7)  ^ ROTR(x,18) ^ ((x) >> 3))
# define SHA256_s1(x)  (ROTR(x,17) ^ ROTR(x,19) ^ ((x) >> 10))
```

---

## `sha256_round` — une opération complète

```c
T1 = h + Σ1(e) + Ch(e,f,g) + K[i] + W[i]
T2 = Σ0(a) + Maj(a,b,c)
h  = g
g  = f
f  = e
e  = d + T1
d  = c
c  = b
b  = a
a  = T1 + T2
```

### Macros Sigma (rounds)

```c
# define SHA256_S0(x)  (ROTR(x,2)  ^ ROTR(x,13) ^ ROTR(x,22))
# define SHA256_S1(x)  (ROTR(x,6)  ^ ROTR(x,11) ^ ROTR(x,25))
```

### Macros logiques

```c
# define SHA256_CH(e,f,g)   (((e) & (f)) ^ (~(e) & (g)))
# define SHA256_MAJ(a,b,c)  (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)))
```

---

## Les constantes K[64]

Les 64 constantes sont les 32 premiers bits de la partie fractionnaire
des racines cubiques des 64 premiers nombres premiers.

```c
K[0]  = 0x428a2f98   // ∛2
K[1]  = 0x71374491   // ∛3
K[2]  = 0xb5c0fbcf   // ∛5
// ...
K[63] = 0xc67178f2   // ∛311
```

---

## `sha256_transform` — pipeline complet

```c
void    sha256_transform(t_sha256_ctx *ctx)
{
    uint32_t        W[64];
    t_sha256_regs   r;
    int             i;

    build_schedule(W, ctx->block.words);   // étendre le schedule
    i = 0;
    while (i < 8)
    {
        r.r[i] = ctx->h[i];               // copier H dans registres
        i++;
    }
    sha256_rounds(&r, W);                  // 64 rounds
    i = 0;
    while (i < 8)
    {
        ctx->h[i] += r.r[i];              // addition finale (mod 2^32)
        i++;
    }
}
```

---

## Différence MD5 vs SHA-256

| | MD5 | SHA-256 |
|---|---|---|
| Registres | 4 (A B C D) | 8 (H0..H7) |
| Schedule | M[i] directs | W[i] étendus de 16 à 64 |
| Rounds | 64 en 4 groupes | 64 identiques |
| Endianness | little-endian | big-endian (SWAP) |
| Fonctions | F G H I | Ch Maj S0 S1 s0 s1 |

---

## Fichiers

```
src/module/sha256/sha256_transform.c
src/include/algorithme.h  → macros ROTR, SWAP, S0, S1, s0, s1, CH, MAJ
src/include/digest.h      → t_sha256_regs, t_sha256_ctx
```