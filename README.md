# ft_ssl [md5] [sha256]

Réimplémentation partielle d'OpenSSL en C — projet 42.
Supporte les algorithmes de hachage MD5 et SHA-256 avec leurs options d'affichage.

---

## Usage

```bash
./ft_ssl md5    [flags] [fichier...] [-s string...]
./ft_ssl sha256 [flags] [fichier...] [-s string...]
```

### Flags

| Flag | Description |
|------|-------------|
| `-p` | Lire STDIN et afficher le contenu entre parenthèses avant le hash |
| `-q` | Mode silencieux — afficher uniquement le hash |
| `-r` | Mode inversé — afficher le hash avant le label |
| `-s` | Hasher la string fournie en argument |

### Exemples

```bash
# STDIN
echo -n "abc" | ./ft_ssl md5
# MD5(stdin)= 900150983cd24fb0d6963f7d28e17f72

# String
./ft_ssl md5 -s "foo"
# MD5("foo")= acbd18db4cc2f85cedef654fccc4a4d8

# Fichier
./ft_ssl sha256 file.txt
# SHA2-256(file.txt)= ba7816bf...

# Flags combinés
echo "42 is nice" | ./ft_ssl md5 -p -r -s "foo" file.txt
```

---

## Architecture

```
ft_ssl
├── parsing         → parse_args, check_args, free_args
├── input           → input_from_string, input_from_fd, input_from_stdin
├── dispatch        → tableau t_hasher[] + dispatch()
├── run_hash        → orchestration stdin / strings / fichiers
├── display_hash    → formatage selon les flags
├── md5/            → init, transform, process, padding, final
└── sha256/         → init, transform, process, padding, final
```

### Principe de scalabilité

Le système de dispatch utilise un tableau de structs avec pointeurs de fonctions.
Ajouter un nouvel algorithme ne nécessite qu'une ligne :

```c
static const t_hasher   hashers[] = {
    { "md5",    "MD5",     compute_md5    },
    { "sha256", "SHA2-256", compute_sha256 },
    { NULL,     NULL,      NULL            }
};
```

---

## Structure du projet

```
ft_ssl_md5_sha256/
├── main.c
├── Makefile
├── docs/                          ← documentation par module
├── src/
│   ├── include/                   ← headers
│   │   ├── algorithme.h           ← macros MD5 + SHA-256
│   │   ├── args.h                 ← t_args, t_flags, t_file
│   │   ├── block.h                ← t_block32, t_block64
│   │   ├── digest.h               ← t_md5_ctx, t_sha256_ctx, t_sha256_regs
│   │   ├── hash.h                 ← t_hash_fn, t_hasher, t_display
│   │   ├── input.h                ← t_input
│   │   ├── md5.h                  ← déclarations MD5
│   │   ├── serialize.h            ← t_serial
│   │   ├── sha256.h               ← déclarations SHA-256
│   │   └── utils.h                ← ft_calloc, ft_realloc, copy_bytes...
│   └── module/
│       ├── algohash/              ← dispatch, run_hash, display_hash
│       ├── args/                  ← parsing des arguments
│       ├── input/                 ← lecture stdin, fd, string
│       ├── md5/                   ← implémentation MD5
│       ├── serialize/             ← sérialisation des blocs
│       ├── sha256/                ← implémentation SHA-256
│       └── utils/                 ← utilitaires string + mémoire
└── test/
    ├── Makefile
    ├── test_ft_ssl_md5.sh         ← 33 tests MD5 vs OpenSSL
    ├── ft_ssl_sha256.sh           ← 45 tests SHA-256 vs OpenSSL
    ├── md5/                       ← tests unitaires MD5
    ├── sha256/                    ← tests unitaires SHA-256
    ├── args/, input/, serialize/  ← tests unitaires des modules
    └── algohash/                  ← tests display_hash
```

---

## Contraintes techniques

- Langage C uniquement
- Fonctions autorisées : `open` `close` `read` `write` `malloc` `free`
- Interdits : `printf` `sprintf` `memcpy` `strlen` `strcmp` et toute libc
- Pas de `if/else` chaîné pour le dispatch → pointeurs de fonctions obligatoires

---

## Compilation

```bash
make        # compiler ft_ssl
make test   # lancer tous les tests unitaires
make re     # recompiler depuis zéro
make clean  # supprimer les objets
make fclean # supprimer les objets et ft_ssl
```

---

## Tests

```bash
# Tests d'intégration MD5 (33 tests vs OpenSSL)
cd test && ./test_ft_ssl_md5.sh

# Tests d'intégration SHA-256 (45 tests vs OpenSSL)
cd test && ./ft_ssl_sha256.sh

# Tests unitaires
cd test && make
```

### Résultats

```
MD5    : 33/33 tests passent contre OpenSSL ✓
SHA256 : 45/45 tests passent contre OpenSSL ✓
```

---

## Algorithmes

### MD5 (RFC 1321)

- Hash de 128 bits (32 chars hex)
- 4 registres 32 bits (A B C D)
- 64 opérations en 4 rounds de 16
- Little-endian
- Padding : 0x80 + zéros jusqu'à 56 octets + taille 8 octets little-endian

→ [docs/md5.md](docs/md5.md) |
[docs/transformation_md5.md](docs/transformation_md5.md) |
[docs/padding_md5.md](docs/padding_md5.md)

### SHA-256 (RFC 6234)

- Hash de 256 bits (64 chars hex)
- 8 registres 32 bits (H0..H7)
- 64 opérations avec message schedule étendu (16 → 64 mots)
- Big-endian
- Padding : 0x80 + zéros jusqu'à 56 octets + taille 8 octets big-endian

→ [docs/sh256_init.md](docs/sh256_init.md) |
[docs/sha256_transform.md](docs/sha256_transform.md) |
[docs/sha256_process.md](docs/sha256_process.md) |
[docs/sha256_padding.md](docs/sha256_padding.md) |
[docs/sha256.md](docs/sha256.md)

---

## Documentation par module

| Module | Fichier | Description |
|--------|---------|-------------|
| Parsing | [docs/parsing.md](docs/parsing.md) | parse_args, flags, fichiers, strings |
| Sérialisation | [docs/Sérialisation.md](docs/Sérialisation.md) | serialize_bytes, t_serial |
| Dispatch | [docs/dispatch.md](docs/dispatch.md) | t_hasher[], dispatch(), scalabilité |
| Run Hash | [docs/run_hash.md](docs/run_hash.md) | orchestration stdin/strings/fichiers |
| Display Hash | [docs/display_hash.md](docs/display_hash.md) | formatage -p -q -r |
| MD5 | [docs/md5.md](docs/md5.md) | pipeline complet MD5 |
| MD5 Transform | [docs/transformation_md5.md](docs/transformation_md5.md) | 64 opérations, macros |
| MD5 Padding | [docs/padding_md5.md](docs/padding_md5.md) | little-endian, cas limites |
| SHA-256 Init | [docs/sh256_init.md](docs/sh256_init.md) | valeurs RFC, H0..H7 |
| SHA-256 Transform | [docs/sha256_transform.md](docs/sha256_transform.md) | schedule, rounds, K[64] |
| SHA-256 Process | [docs/sha256_process.md](docs/sha256_process.md) | streaming API, msg_size |
| SHA-256 Padding | [docs/sha256_padding.md](docs/sha256_padding.md) | big-endian, messages longs |
| SHA-256 Final | [docs/sha256.md](docs/sha256.md) | word_to_hex, vecteurs RFC |
| Erreurs | [docs/erreurs_contraintes.md](docs/erreurs_contraintes.md) | 12 bugs documentés |

---

## Bugs résolus

| # | Module | Bug |
|---|--------|-----|
| 1 | ft_strcmp | Comparaison incorrecte après `\0` |
| 2 | ft_realloc | Arguments src/dst inversés |
| 3 | compute_md5 | md5_init appelé après serialize |
| 4 | compute_md5 | Messages > 64 octets tronqués |
| 5 | display_hash | Label stdin pour toutes les entrées en mode quiet |
| 6 | write_label | `\n` affiché dans le label `-p` |
| 7 | ft_calloc | Typo `sstatic` |
| 8 | headers | `#include <stdio.h>` interdit |
| 9 | algorithme.h | Conflit de nom `MD5_H` |
| 10 | compute_md5 | Compound literal → comportement indéfini |
| 11 | tests | `ctx` non initialisé → valeurs aléatoires |
| 12 | openssl | `openssl md5 -s` cassé sur certaines versions |

→ [docs/erreurs_contraintes.md](docs/erreurs_contraintes.md)

---

## Auteur

Projet 42 — ft_ssl [md5] [sha256]