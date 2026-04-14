# Documentation — SHA-256 Process

---

## Rôle

`sha256_process` est l'interface principale pour alimenter des données
dans le contexte SHA-256. Elle gère automatiquement les frontières de
blocs et appelle `sha256_transform` quand un bloc de 64 octets est complet.

---

## Signature

```c
int     sha256_process(t_sha256_ctx *ctx, const void *data, uint64_t len);
```

---

## Flux d'exécution

```
données entrantes (len octets)
    ↓
msg_size += len * 8          (accumulation en bits)
    ↓
si bloc partiellement rempli (used > 0)
    → compléter le bloc
    → si bloc plein → sha256_transform → used = 0
    ↓
tant que données restantes >= 64
    → copier 64 octets → sha256_transform
    ↓
copier le reste dans le bloc courant → used = reste
```

---

## Implémentation

```c
int     sha256_process(t_sha256_ctx *ctx, const void *data, uint64_t len)
{
    const uint8_t   *ptr;
    uint64_t        fill;

    if (!ctx || (!data && len > 0))
        return (0);
    ptr = (const uint8_t *)data;
    ctx->msg_size += len * 8;
    if (ctx->used > 0)
    {
        fill = 64 - ctx->used;
        if (len < fill)
        {
            copy_bytes(ctx->block.bytes + ctx->used, (uint8_t *)ptr, (int)len);
            ctx->used += len;
            return (1);
        }
        copy_bytes(ctx->block.bytes + ctx->used, (uint8_t *)ptr, (int)fill);
        sha256_transform(ctx);
        ptr += fill; len -= fill; ctx->used = 0;
    }
    while (len >= 64)
    {
        copy_bytes(ctx->block.bytes, (uint8_t *)ptr, 64);
        sha256_transform(ctx);
        ptr += 64; len -= 64;
    }
    if (len > 0)
    {
        copy_bytes(ctx->block.bytes, (uint8_t *)ptr, (int)len);
        ctx->used = len;
    }
    return (1);
}
```

---

## `msg_size` — accumulation en bits

```c
ctx->msg_size += len * 8;
```

`msg_size` accumule la taille totale en **bits** — pas en octets.
Ce choix simplifie `sha256_padding` qui encode la taille en bits
directement.

```
"abc" (3 octets)  → msg_size = 24
64 octets         → msg_size = 512
200 octets        → msg_size = 1600
```

---

## Gestion des messages > 64 octets

```
message de 200 octets :

used = 0 au départ
→ while (len >= 64) → 3 iterations
    iter 1 : copy 64 → transform → ptr += 64, len = 136
    iter 2 : copy 64 → transform → ptr += 64, len = 72
    iter 3 : copy 64 → transform → ptr += 64, len = 8
→ copy 8 octets restants → used = 8
```

---

## Appels depuis sha256_padding

`sha256_process` est aussi appelée pendant le padding pour ajouter
les octets `0x80`, `0x00`, et les octets de taille.

Ces appels **accumulent dans msg_size** — mais `sha256_padding`
sauvegarde `bit_len = ctx->msg_size` **avant** d'appeler process,
donc la taille encodée dans le padding est correcte.

---

## Différence avec MD5

MD5 utilise `serialize_bytes` — une fonction qui remplit le bloc mais
ne déclenche pas automatiquement `md5_process`. `compute_md5` doit
gérer manuellement la boucle de blocs.

SHA-256 utilise `sha256_process` — une interface streaming qui gère
tout automatiquement. `compute_sha256` fait juste :

```c
sha256_init(ctx);
sha256_process(ctx, input->data, input->size);
hash = sha256(ctx);
```

---

## Protections

```
ctx NULL + data NULL + len > 0  → return 0
ctx NULL                         → return 0
data NULL + len = 0             → return 1 (no-op)
```

---

## Fichiers

```
src/module/sha256/sha256_process.c
src/include/sha256.h    → déclaration
src/include/utils.h     → copy_bytes
```