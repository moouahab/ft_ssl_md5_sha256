# Documentation — SHA-256 Padding

---

## Rôle

`sha256_padding` ajoute les données de complétion au message pour
que sa longueur totale soit un multiple de 512 bits (64 octets),
et encode la taille originale du message dans les 8 derniers octets.

---

## Schéma du padding

```
message : [données][0x80][0x00...][taille 8 octets big-endian]
                         ←  padding  →
```

Pour un message de N octets :

```
position 0..N-1     → données originales
position N          → 0x80 (bit '1' suivi de zéros)
position N+1..55    → 0x00 (complétion jusqu'à l'octet 56)
position 56..63     → taille en bits sur 8 octets big-endian
```

---

## Implémentation

```c
int     sha256_padding(t_sha256_ctx *ctx)
{
    uint64_t    bit_len;
    uint8_t     byte;
    int         i;

    if (!ctx)
        return (0);
    bit_len = ctx->msg_size;           // sauvegarder AVANT corruption
    sha256_process(ctx, "\x80", 1);    // ajouter le bit '1'
    while (ctx->used != 56)            // remplir jusqu'à 56 octets
        sha256_process(ctx, "\0", 1);
    i = 7;
    while (i >= 0)                     // encoder taille big-endian
    {
        byte = (bit_len >> (i * 8)) & 0xFF;
        sha256_process(ctx, &byte, 1);
        i--;
    }
    return (1);
}
```

---

## Pourquoi sauvegarder `bit_len` avant le padding

`sha256_process` accumule `msg_size` à chaque appel. Les appels
de padding (0x80, 0x00) corrompraient la taille si on ne la sauvegardait
pas d'abord.

```c
bit_len = ctx->msg_size;        // = 24 pour "abc"
sha256_process(ctx, "\x80", 1); // msg_size += 8 → 32 (corrompu)
// ...
byte = (bit_len >> (i * 8)) & 0xFF;  // utilise 24, pas 32 ✓
```

---

## Encodage de la taille — big-endian

SHA-256 encode la taille du message en **big-endian** sur 8 octets.
MD5 l'encode en **little-endian**.

```
"abc" = 3 octets = 24 bits = 0x0000000000000018

big-endian (SHA-256) :
bytes[56] = 0x00
bytes[57] = 0x00
bytes[58] = 0x00
bytes[59] = 0x00
bytes[60] = 0x00
bytes[61] = 0x00
bytes[62] = 0x00
bytes[63] = 0x18   ← octet le moins significatif en dernier
```

La boucle `i = 7..0` encode l'octet le plus significatif en premier :

```c
i=7 : byte = (24 >> 56) & 0xFF = 0x00  → bytes[56]
i=6 : byte = (24 >> 48) & 0xFF = 0x00  → bytes[57]
...
i=0 : byte = (24 >>  0) & 0xFF = 0x18  → bytes[63]
```

---

## Gestion des messages longs (>= 56 octets)

Si le message est >= 56 octets, `0x80` + les données existantes
dépassent la position 56 → il faut un deuxième bloc de padding.

La boucle `while (ctx->used != 56)` gère ce cas automatiquement :
quand `sha256_process` remplit le bloc à 64 octets, il appelle
`sha256_transform` et reset `used = 0`. La boucle continue
alors jusqu'à `used == 56` dans le nouveau bloc.

```
message 57 octets :

after 0x80   : used = 58 (> 56)
zeros added  : used = 59, 60, 61, 62, 63
add zero     : bloc plein → transform → used = 0
zeros added  : used = 1, 2 ... 56 ✓
length added : used = 57, 58 ... 64 → transform → used = 0
```

---

## Comparaison MD5 vs SHA-256

| | MD5 | SHA-256 |
|---|---|---|
| Bit de padding | 0x80 | 0x80 |
| Remplissage | jusqu'à 56 octets | jusqu'à 56 octets |
| Taille encodée | little-endian 8 octets | big-endian 8 octets |
| Position taille | words[14]=bas words[15]=haut | i=7..0 MSB en premier |

---

## Fichiers

```
src/module/sha256/sha256_padding.c
src/include/sha256.h    → déclaration
```