# Documentation — Module de Sérialisation `ft_ssl`

---

## Pourquoi la sérialisation ?

Les algorithmes de hachage (MD5, SHA-256, SHA-512) ne travaillent pas sur des
strings ou des fichiers directement. Ils travaillent sur des **octets bruts**
organisés en blocs de taille fixe.

La sérialisation est la conversion de n'importe quelle source de données
(string, fichier, STDIN) en une séquence d'octets exploitable par l'algorithme.

```
"abc"          → [0x61][0x62][0x63]
fichier.txt    → [octets bruts lus par read()]
STDIN          → [octets bruts lus par read()]
        ↓
même représentation → même traitement
```

---

## Les sources de données

### String
```c
char *str = "abc";
uint8_t *bytes = (uint8_t *)str;
// 'a' = 0x61, 'b' = 0x62, 'c' = 0x63
```
Un `char` est déjà un octet — le cast suffit.

### Fichier / STDIN
```c
uint8_t buffer[64];
read(fd, buffer, 64);    // fichier
read(0,  buffer, 64);    // STDIN (fd=0)
```
`read` retourne directement des octets bruts.

---

## Structure `t_input` — unification des sources

Pour éviter 3 fonctions différentes dans l'algo, toutes les sources
sont converties en `t_input` avant traitement :

```c
typedef struct  s_input
{
    uint8_t     *data;    // pointeur vers les octets
    uint64_t    size;     // taille en octets
}               t_input;
```

```
string  → t_input { data = (uint8_t *)str,  size = ft_strlen(str) }
fichier → t_input { data = buffer lu,        size = octets lus     }
STDIN   → t_input { data = buffer lu,        size = octets lus     }
```

L'algorithme reçoit toujours un `t_input` — il ne sait pas d'où viennent les données.

---

## Les unions — deux vues sur le même bloc mémoire

### Le problème sans union

MD5 a besoin de deux accès différents sur le même bloc :
- **Octet par octet** → pour le padding
- **Mot par mot (32 bits)** → pour l'algorithme

Sans union il faudrait une conversion manuelle :
```c
// conversion manuelle → lente et risquée
words[i] = (uint32_t)bytes[i*4]
         | (uint32_t)bytes[i*4+1] << 8
         | (uint32_t)bytes[i*4+2] << 16
         | (uint32_t)bytes[i*4+3] << 24;
```

### La solution — union

```c
typedef union   u_block32
{
    uint8_t     bytes[64];    // vue octet par octet
    uint32_t    words[16];    // vue mot par mot (32 bits)
}               t_block32;
```

Le même bloc mémoire vu de deux façons :
```
mémoire : [61][62][63][64][65][66][67][68]...
               ↓                   ↓
        bytes[0..3]           bytes[4..7]
               ↓                   ↓
           words[0]             words[1]
         0x64636261           0x68676665
```

### Les unions disponibles

```c
// MD5 et SHA-256 — bloc 64 octets, mots 32 bits
typedef union   u_block32
{
    uint8_t     bytes[64];
    uint32_t    words[16];
}               t_block32;

// SHA-512 — bloc 128 octets, mots 64 bits
typedef union   u_block64
{
    uint8_t     bytes[128];
    uint64_t    words[16];
}               t_block64;

// Whirlpool — bloc 64 octets, mots 64 bits
typedef union   u_block_whirlpool
{
    uint8_t     bytes[64];
    uint64_t    words[8];
}               t_block_whirlpool;
```

---

## Les contextes — état complet du hachage

Chaque algorithme a son propre contexte qui regroupe tout ce dont
il a besoin pendant le traitement :

```c
typedef struct  s_md5_ctx
{
    t_block32   block;      // bloc courant (union bytes/words)
    size_t      used;       // octets valides dans le bloc (0 à 64)
    uint64_t    msg_size;   // taille totale du message en octets
    uint32_t    a[4];       // registres d'état A, B, C, D
}               t_md5_ctx;

typedef struct  s_sha256_ctx
{
    t_block32   block;
    size_t      used;
    uint64_t    msg_size;
    uint32_t    h[8];       // registres H0 à H7
}               t_sha256_ctx;

typedef struct  s_sha512_ctx
{
    t_block64   block;
    size_t      used;
    uint64_t    msg_size;
    uint64_t    h[8];       // registres H0 à H7 (64 bits)
}               t_sha512_ctx;
```

### Accès aux registres via macros

```c
// MD5
# define MD5_A(ctx)     (ctx)->a[0]
# define MD5_B(ctx)     (ctx)->a[1]
# define MD5_C(ctx)     (ctx)->a[2]
# define MD5_D(ctx)     (ctx)->a[3]

// SHA-256 / SHA-512
# define SHA256_H0(ctx) (ctx)->h[0]
# define SHA256_H1(ctx) (ctx)->h[1]
# define SHA256_H2(ctx) (ctx)->h[2]
# define SHA256_H3(ctx) (ctx)->h[3]
# define SHA256_H4(ctx) (ctx)->h[4]
# define SHA256_H5(ctx) (ctx)->h[5]
# define SHA256_H6(ctx) (ctx)->h[6]
# define SHA256_H7(ctx) (ctx)->h[7]
```

---

## Les macros — pourquoi et comment

### Le problème sans macros

Dans l'algorithme MD5, les registres A B C D apparaissent des dizaines de fois.
Sans macros le code devient illisible et ne correspond plus à la documentation :

```c
// illisible — indices sans signification
ctx->a[0] = ctx->a[1] + ROTATE(
    ctx->a[0] + F(ctx->a[1], ctx->a[2], ctx->a[3]) + M[i] + T[i], s);
```

### Avec macros — correspond à la spec MD5

```c
// lisible — correspond exactement à la documentation officielle
MD5_A(ctx) = MD5_B(ctx) + ROTATE(
    MD5_A(ctx) + F(MD5_B(ctx), MD5_C(ctx), MD5_D(ctx)) + M[i] + T[i], s);
```

### Les 3 raisons d'utiliser les macros

**1. Lisibilité**
Le code correspond mot pour mot à la documentation officielle MD5/SHA.
Un relecteur peut vérifier l'implémentation directement contre la spec.

**2. Maintenabilité**
Si la structure interne change, on modifie uniquement la macro :
```c
// avant
# define MD5_A(ctx)  (ctx)->a[0]

// après changement de structure — une seule ligne à modifier
# define MD5_A(ctx)  (ctx)->regs[0]
```

**3. Sécurité**
Le ctx est passé en paramètre de la macro — le compilateur vérifie le type :
```c
// DANGEREUX — suppose que la variable s'appelle toujours "ctx"
# define MD5_A   ctx->a[0]

// SÛR — fonctionne peu importe le nom de la variable
# define MD5_A(ctx)   (ctx)->a[0]
```

### Pourquoi pas des variables locales

Une alternative serait de copier les registres en variables locales :
```c
uint32_t a = ctx->a[0];
uint32_t b = ctx->a[1];
uint32_t c = ctx->a[2];
uint32_t d = ctx->a[3];
// ... algo ...
// resynchronisation obligatoire à la fin
ctx->a[0] = a;
ctx->a[1] = b;
ctx->a[2] = c;
ctx->a[3] = d;  // ← risque d'oublier → bug silencieux
```

Avec les macros on travaille **directement** sur le ctx :
```
pas de copie → pas de synchro → pas de risque d'oubli
```

---

## Structure `t_serial` — sérialisation générique

Pour éviter de dupliquer la logique de copie dans chaque ctx :

```c
typedef struct  s_serial
{
    uint8_t     *bytes;       // destination (block.bytes du ctx)
    size_t      *used;        // compteur d'octets utilisés
    size_t      block_size;   // 64 ou 128 selon l'algo
}               t_serial;
```

---

## Fonction `serialize_bytes` — le coeur

```c
static void     serialize_bytes(t_serial *s, uint8_t *src, uint64_t size)
```

### Protections
```
!s || !src || !size    → entrées invalides → retour immédiat
space = block_size - used → calcul de la place restante
size > space           → tronque à la place disponible (pas de dépassement)
```

### Optimisation bitwise
```
si used est aligné sur 8 ET il reste >= 8 octets
    → copie 8 octets à la fois via uint64_t
sinon
    → copie octet par octet
```

```c
// copie 8 octets en une opération
*(uint64_t *)(s->bytes + *s->used) = *(uint64_t *)(src + i);
*s->used += 8;
i += 8;
```

### Pourquoi l'alignement est important
```
uint64_t * doit pointer sur une adresse multiple de 8
si used % 8 != 0 → accès non aligné → comportement indéfini
```

---

## Fonctions de sérialisation par algorithme

```c
t_md5_ctx    *serialize_md5(t_input *input);
t_sha256_ctx *serialize_sha256(t_input *input);
t_sha512_ctx *serialize_sha512(t_input *input);
```

### Protections communes
```
!input || !input->data || !input->size   → NULL
input->size > 0x1FFFFFFFFFFFFFFF         → overflow impossible
ft_calloc échoue                         → NULL
serialize_bytes échoue                   → free(ctx) + NULL
```

### Différences entre les fonctions

| Fonction | block_size | ctx type | max_size |
|----------|-----------|----------|----------|
| serialize_md5 | 64 | t_md5_ctx | 0x1FFF... |
| serialize_sha256 | 64 | t_sha256_ctx | 0x1FFF... |
| serialize_sha512 | 128 | t_sha512_ctx | 0x3FFF... |

---

## Endianness — pourquoi ça marche

MD5 et SHA-256 travaillent en **little-endian** — le premier octet
est le moins significatif.

```
"abcd" en mémoire : [0x61][0x62][0x63][0x64]
words[0] little-endian : 0x64636261
                              ↑
                    'd' est l'octet le plus significatif
```

L'union lit directement la mémoire sans conversion → sur x86 (little-endian)
le résultat est correct pour MD5 et SHA-256.

---

## Schéma global

```
source (string / fichier / STDIN)
          ↓
      t_input { data, size }
          ↓
  serialize_md5(input)
          ↓
  t_md5_ctx alloué via ft_calloc
          ↓
  t_serial { bytes, used, block_size=64 }
          ↓
  serialize_bytes(&s, input->data, input->size)
          ↓
  ctx->block.bytes[] → données copiées
  ctx->block.words[] → accès 32 bits via union
  ctx->used          → octets valides
  ctx->msg_size      → taille totale
          ↓
  prêt pour le padding
```

---

## Fichiers du module

```
src/
└── module/
    └── serialize/
        ├── serialize_bytes.c   → t_serial + serialize_bytes (générique)
        └── serialize.c         → serialize_md5, serialize_sha256, serialize_sha512
src/include/
    ├── block.h                 → t_block32, t_block64, t_block_whirlpool
    ├── digest.h                → t_md5_ctx, t_sha256_ctx, t_sha512_ctx
    ├── input.h                 → t_input
    └── algorithme.h            → macros registres + includes
```

---

## Gestion mémoire

| Pointeur | Alloué par | Libéré par |
|----------|-----------|-----------|
| `ctx` | `ft_calloc` | appelant |
| `ctx->block` | dans ctx (pas séparé) | avec ctx |
| `input->data` (string) | argv (système) | jamais |
| `input->data` (fichier/stdin) | `ft_realloc` | `free_input` |