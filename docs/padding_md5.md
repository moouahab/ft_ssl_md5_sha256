# MD5 | le Padding

---

## Pourquoi le padding existe

MD5 traite les données par blocs de **64 octets exactement** (512 bits).
La réalité est que les messages ont des tailles arbitraires :

```
"abc"              →   3 octets
"hello world"      →  11 octets
un fichier ISO     →  700 000 000 octets
```

Aucun de ces messages n'est un multiple de 64 octets. Le padding est
la solution — il complète le dernier bloc pour qu'il fasse exactement
64 octets, tout en encodant des informations essentielles.

---

## Ce que le padding encode

Le padding ajoute trois choses dans cet ordre :

```
[message original][marqueur de fin][rembourrage][taille du message]
                   ↑                ↑            ↑
                 0x80            zéros        8 octets
```

### 1. Le marqueur de fin — 0x80

Un seul octet `0x80` (soit `10000000` en binaire) est placé juste après
le dernier octet du message.

```
"abc" + padding :
[0x61][0x62][0x63][0x80][...]
  'a'   'b'   'c'   ↑
              marqueur obligatoire
```

Ce marqueur indique à l'algorithme où s'arrête le vrai message.
En binaire, `0x80` représente un bit 1 suivi de 7 zéros — c'est
la convention définie par la RFC 1321.

### 2. Le rembourrage — octets 0x00

Des zéros sont ajoutés pour remplir le bloc jusqu'à la position 56.
En pratique, si le bloc a été alloué avec `ft_calloc`, ces zéros
sont **déjà en place** — pas besoin de les écrire explicitement.

```
positions 0  à msg_size - 1 : données du message
position  msg_size           : 0x80
positions msg_size + 1 à 55  : 0x00 (déjà à zéro)
positions 56 à 63            : taille en bits
```

### 3. La taille du message — 8 derniers octets

Les 8 derniers octets (positions 56 à 63) contiennent la taille
**originale** du message en **bits** (pas en octets), encodée en
**little-endian sur 64 bits**.

```
msg_size = 3 octets
taille en bits = 3 × 8 = 24 = 0x18

position 56 : 0x18   ← octet le moins significatif
position 57 : 0x00
position 58 : 0x00
position 59 : 0x00
position 60 : 0x00
position 61 : 0x00
position 62 : 0x00
position 63 : 0x00   ← octet le plus significatif
```

En code, grâce à l'union `t_block32`, on écrit directement :

```c
uint64_t bits = msg_size * 8;
ctx->block.words[14] = (uint32_t)(bits);         // bits 0-31
ctx->block.words[15] = (uint32_t)(bits >> 32);   // bits 32-63
```

---

## Exemple complet — "abc" (3 octets)

```
bloc de 64 octets après padding :

index : 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15
valeur: 61 62 63 80 00 00 00 00 00 00 00 00 00 00 00 00
         a   b   c  ↑  ← zéros ─────────────────────→
                  0x80

index : 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
valeur: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        ← zéros ───────────────────────────────────────

index : 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47
valeur: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        ← zéros ───────────────────────────────────────

index : 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
valeur: 00 00 00 00 00 00 00 00 18 00 00 00 00 00 00 00
        ← zéros ─────────────→  ↑  ← zéros ──────────
                               taille = 24 bits
```

Vu comme 16 mots de 32 bits :

```
words[0]  = 0x80636261   ('a','b','c',0x80 en little-endian)
words[1]  = 0x00000000
...
words[13] = 0x00000000
words[14] = 0x00000018   (24 = 3 × 8 bits)
words[15] = 0x00000000
```

---

## Les 3 cas de padding

### Cas 1 — Message court (used ≤ 55)

Le padding tient dans le bloc courant en une seule passe.

```
used = 3 ("abc")

[a][b][c][0x80][0x00 ... 0x00][taille]
 0   1   2   3    4        55  56  63
                 ↑
           déjà à zéro
```

```c
ctx->block.bytes[ctx->used] = 0x80;   // position 3
ctx->used++;                           // used = 4
// used (4) ≤ 56 → pas de process intermédiaire
write_size(ctx);                       // écrit en [56-63]
```

---

### Cas 2 — Message à la limite (used = 55)

Le 0x80 va en position 55, used passe à 56 — exactement à la limite.
Il n'y a pas de place pour la taille dans le même bloc.

```
used = 55

[données ... 54 octets][0x80][taille]
 0                      55   56  63
                         ↑
                     0x80 en [55]
```

```c
ctx->block.bytes[55] = 0x80;   // used = 55
ctx->used++;                    // used = 56
// used (56) > 56 → FAUX (56 n'est pas > 56)
// donc pas de process → write_size directement
write_size(ctx);                // écrit en [56-63]
```

Attention : la condition est `used > 56`, pas `used >= 56`.
Donc `used = 56` après le 0x80 → `write_size` directement.

---

### Cas 3 — Message long (used > 55)

Le 0x80 ne laisse pas assez de place pour la taille dans le bloc courant.
Il faut traiter le bloc courant et créer un nouveau bloc pour la taille.

```
used = 60

Bloc courant :
[données ... 60 octets][0x80][0x00][0x00][0x00]
 0                      60    61    62    63

→ traiter ce bloc (md5_process)
→ bloc réinitialisé à zéro, used = 0

Nouveau bloc :
[0x00 ... 0x00][taille]
 0          55  56  63
```

```c
ctx->block.bytes[60] = 0x80;   // used = 60
ctx->used++;                    // used = 61
// used (61) > 56 → VRAI
md5_process(ctx);               // traite le bloc + used = 0
// nouveau bloc vide
write_size(ctx);                // écrit en [56-63] du nouveau bloc
```

---

### Cas 4 — Bloc plein (used = 64)

Si le bloc est complètement plein avant le padding, on le traite
immédiatement puis on continue.

```
used = 64

[données ... 64 octets] ← bloc plein

→ md5_process immédiat
→ bloc réinitialisé, used = 0
→ puis padding normal
```

```c
if (ctx->used >= 64)
    md5_process(ctx);   // vide le bloc avant tout
// now used = 0 → padding normal
ctx->block.bytes[0] = 0x80;
// ...
```

---

## Tableau récapitulatif des cas

| `used` avant padding | 0x80 en position | Process intermédiaire | Taille en position |
|---------------------|-----------------|----------------------|-------------------|
| 0 à 54              | `used`          | Non                  | 56 à 63           |
| 55                  | 55              | Non                  | 56 à 63           |
| 56 à 63             | `used`          | Oui → nouveau bloc   | 56 à 63           |
| 64                  | 0 (après reset) | Oui → puis normal    | 56 à 63           |

---

## Pourquoi encoder la taille ?

La taille est encodée pour éviter les **attaques par extension de longueur**.

Sans taille encodée, deux messages différents pourraient produire le
même bloc paddé si l'un est le préfixe de l'autre avec du padding naturel.

Avec la taille, le bloc final est unique pour chaque longueur de message.

---

## Pourquoi 0x80 et pas 0x01 ou 0xFF ?

`0x80` = `10000000` en binaire.

C'est la convention choisie par la RFC 1321. Le bit 1 suivi de zéros
signifie clairement "ici commence le padding". C'est une valeur qui
ne peut pas apparaître naturellement comme continuation d'un message
dans le contexte binaire de MD5.

---

## Implémentation — les fonctions

```c
// écrire la taille en little-endian aux positions 56-63
static void     write_size(t_md5_ctx *ctx)
{
    uint64_t    bits;

    if (!ctx || ctx->msg_size > 0x1FFFFFFFFFFFFFFF)
        return ;
    bits = ctx->msg_size * 8;
    ctx->block.words[14] = (uint32_t)(bits);
    ctx->block.words[15] = (uint32_t)(bits >> 32);
}

// fonction principale de padding
void            md5_padding(t_md5_ctx *ctx)
{
    if (!ctx)
        return ;
    if (ctx->used >= 64)          // cas 4 — bloc plein
        md5_process(ctx);
    ctx->block.bytes[ctx->used] = 0x80;
    ctx->used++;
    if (ctx->used > 56)           // cas 3 — pas assez de place
        md5_process(ctx);
    write_size(ctx);              // cas 1 et 2 — taille directement
}
```

---

## Vérification — tester le padding

Pour vérifier que le padding est correct, on peut comparer avec
la commande OpenSSL :

```bash
echo -n "abc" | openssl md5
# → 900150983cd24fb0d6963f7d28e17f72

echo -n "abc" | ./ft_ssl md5
# → doit donner le même résultat
```

Si le padding est incorrect, le hash sera différent pour des messages
dont la longueur approche 55 ou 64 octets — ce sont les cas limites
à tester en priorité.

---

## Fichiers concernés

```
src/module/md5/
└── md5_padding.c    → write_size + md5_padding

src/include/
└── md5.h            → déclaration void md5_padding(t_md5_ctx *ctx)

test/md5/
└── test_md5_padding.c → tests des 4 cas
```