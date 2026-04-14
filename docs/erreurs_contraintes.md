# Erreurs et Contraintes Rencontrées

---

## 1. `ft_strcmp` — comparaison incorrecte après `\0`

### Symptôme

```
ft_strcmp("abc", "abc") → retournait non-zéro
```

### Cause

`find_diff` continuait à comparer les octets après le `\0` au lieu de
s'arrêter. Les octets suivants (aléatoires en mémoire ou nuls selon
l'allocateur) produisaient une différence.

```c
// FAUX — continue après '\0'
static int  find_diff(uint64_t w1, uint64_t w2, int byte)
{
    if (byte >= 8)
        return (0);
    if (((w1 >> (byte * 8)) & 0xFF) != ((w2 >> (byte * 8)) & 0xFF))
        return (...);
    return (find_diff(w1, w2, byte + 1));  // ← continue même si byte = '\0'
}
```

### Correction

```c
if (b1 == '\0')    // les deux sont '\0' → strings égales
    return (0);
```

---

## 2. `ft_realloc` — arguments `src` et `dst` inversés

### Symptôme

```
input_from_fd() retournait des données corrompues
```

### Cause

```c
// FAUX — copie dst dans src au lieu de src dans dst
copy_bytes((char *)ptr, (char *)new_ptr, copy_size);
```

### Correction

```c
// CORRECT
copy_bytes((uint8_t *)new_ptr, (uint8_t *)ptr, copy_size);
```

---

## 3. `md5_init` — appelé après `serialize_md5`

### Symptôme

```
MD5("abc") → d41d8cd98f00b204e9800998ecf8427e  (hash de chaîne vide)
```

### Cause

`md5_init` zero-initialisait le bloc après que `serialize_md5` y avait
copié les données — les données étaient écrasées.

```c
// FAUX
ctx = serialize_md5(input);   // copie "abc" dans le bloc
md5_init(ctx);                // ← efface tout
```

### Correction

```c
// CORRECT — init avant serialize
ctx = ft_calloc(1, sizeof(t_md5_ctx));
md5_init(ctx);                // registres A B C D
ctx->msg_size = input->size;
serialize_bytes(...);         // copie les données
```

---

## 4. `compute_md5` — messages > 64 octets tronqués

### Symptôme

```
MD5 de 65 chars → hash incorrect
MD5 de 64 chars → hash correct
```

### Cause

`serialize_bytes` est limitée à `block_size = 64` octets. Les octets
au-delà de 64 étaient silencieusement ignorés.

```c
// FAUX — copie au maximum 64 octets
serialize_bytes(&s, input->data, input->size);
// pour input->size = 200 → seuls les 64 premiers octets sont traités
```

### Correction

Traiter le message **bloc par bloc** :

```c
while (offset < input->size)
{
    serialize_bytes(&s, input->data + offset, input->size - offset);
    offset += ctx->used;
    if (ctx->used == 64)
        md5_process(ctx);   // traiter le bloc complet
    else
        break ;              // dernier bloc partiel
}
```

---

## 5. `display_hash` — label stdin affiché pour toutes les entrées

### Symptôme

```
./ft_ssl md5 -r -q -p -s "foo" file
→ affichait "foo" et "file" en plus du hash
```

### Cause

```c
if (disp->quiet)
{
    if (disp->print_stdin)   // ← s'exécutait pour TOUTES les entrées
    {
        write(1, label, ft_strlen(label));  // affichait "foo", "file"...
        write(1, "\n", 1);
    }
    ft_putstr(hash, 1);
    ft_putstr("\n", 1);
    return ;
}
```

### Correction

`display_hash` gère uniquement strings et fichiers. Stdin est géré
séparément par `display_stdin_hash` dans `run_hash.c`.

```c
if (disp->quiet)
{
    ft_putstr(hash, 1);
    ft_putstr("\n", 1);
    return ;              // ← plus de logique stdin ici
}
```

---

## 6. `-p` affichait `\n` littéral dans le label

### Symptôme

```
echo "abc" | ./ft_ssl md5 -p
→ ("abc\n")= hash   ← \n visible dans le label
→ attendu : ("abc")= hash
```

### Cause

`write_label` ne retirait pas le `\n` final du contenu stdin.

### Correction

```c
static void     write_label(uint8_t *data, uint64_t size, char *buf, int buf_size)
{
    if (size > 0 && data[size - 1] == '\n')
        size--;    // ← retirer le \n final avant formatage
    // ...
}
```

---

## 7. Typo `sstatic` dans `ft_calloc.c`

### Symptôme

```
error: unknown type name 'sstatic'
```

### Cause

```c
sstatic void     zero_aligned(...)   // ← double 's'
```

### Correction

```c
static void     zero_aligned(...)
```

---

## 8. `#include <stdio.h>` interdit par le sujet

### Symptôme

Compilation OK mais utilisation de `printf` interdit.

### Correction

Remplacer tous les `printf` par `write` via `ft_putstr`.
Retirer `<stdio.h>` de tous les headers.

---

## 9. `MD5_H` — conflit de nom entre macro et include guard

### Symptôme

```
// md5.h
#ifndef MD5_H    ← include guard

// algorithme.h
# define MD5_H(b, c, d)  ((b) ^ (c) ^ (d))  ← même nom
```

Conflit — la macro `MD5_H` était définie avant l'include guard.

### Correction

```c
# define MD5_XOR(b, c, d)  ((b) ^ (c) ^ (d))
```

---

## 10. Compound literal `(t_serial){...}` — comportement indéfini

### Symptôme

Résultats instables selon le compilateur.

### Cause

```c
serialize_bytes(&(t_serial){ctx->block.bytes, &ctx->used, 64}, ...);
// prendre l'adresse d'un compound literal → UB selon la norme C99
```

### Correction

```c
t_serial    s;
s.bytes      = ctx->block.bytes;
s.used       = &ctx->used;
s.block_size = 64;
serialize_bytes(&s, ...);
```

---

## 11. `ctx` non initialisé dans les tests

### Symptôme

```
md5_transform → résultats incorrects
```

### Cause

```c
t_md5_ctx   ctx;      // ← stack non initialisée
md5_init(&ctx);       // initialise seulement a[0..3]
                      // block.words[1..13] → valeurs aléatoires
```

### Correction

```c
t_md5_ctx   ctx = {0};   // zero-initialize tout le struct
md5_init(&ctx);
```

---

## 12. `openssl md5 -s` — incompatible avec certaines versions

### Symptôme

```
openssl md5 -s "abc"
→ md5: Unknown option or message digest: s
```

### Cause

Certaines versions récentes d'OpenSSL (`openssl 3.x`) n'acceptent
plus `-s` directement avec `md5`.

### Contournement dans les tests

```bash
# au lieu de openssl md5 -s "abc"
echo -n "abc" | openssl md5 | awk '{print $2}'
```

---

## Tableau récapitulatif

| # | Module | Type | Impact | Corrigé |
|---|--------|------|--------|---------|
| 1 | ft_strcmp | logique | hash incorrect | ✓ |
| 2 | ft_realloc | inversion args | données corrompues | ✓ |
| 3 | compute_md5 | ordre init | hash vide | ✓ |
| 4 | compute_md5 | troncature | hash incorrect > 64 | ✓ |
| 5 | display_hash | logique quiet | affichage parasite | ✓ |
| 6 | write_label | formatage | \n dans label | ✓ |
| 7 | ft_calloc | typo | compilation | ✓ |
| 8 | headers | interdit | compilation | ✓ |
| 9 | algorithme.h | nom conflit | compilation | ✓ |
| 10 | compute_md5 | UB C99 | instabilité | ✓ |
| 11 | tests | init stack | résultats faux | ✓ |
| 12 | tests | openssl version | faux KO | contourné |
