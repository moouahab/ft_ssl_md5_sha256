<!-- Banner -->
<p align="center">
  <img src="./docs/ft_ssl_md5_sha256.png" alt="ft_ssl Cyberpunk Banner" style="width:100%; border-radius: 8px;">
</p>

<h1 align="center">ft_ssl [md5] [sha256]</h1>

<p align="center">
  <img src="https://img.shields.io/badge/42-ft__ssl-00FFE1?style=for-the-badge&logo=42"/>
  <img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c"/>
  <img src="https://img.shields.io/badge/Tests-78%20%2F%2078-success?style=for-the-badge"/>
  <img src="https://img.shields.io/badge/RFC-1321%20%7C%206234-blue?style=for-the-badge"/>
</p>

<p align="center">
  Réimplémentation partielle d'OpenSSL en C — Projet École 42<br>
  Algorithmes de hachage cryptographique MD5 et SHA-256 from scratch
</p>

---

## 🎯 Highlights

```diff
+ ✅ 78 tests validés contre OpenSSL (33 MD5 + 45 SHA-256)
+ 🐛 12 bugs résolus et documentés (serialization, endianness, UB)
+ 🏗️ Architecture scalable par pointeurs de fonctions
+ 📜 Conformité stricte RFC 1321 (MD5) & RFC 6234 (SHA-256)
+ 🔍 Analyse des attack surfaces : timing attacks, length extension, padding oracle
+ ⚡ Zero libc : parsing manuel, gestion mémoire from scratch
```

---

## 📦 Usage

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
# SHA2-256(file.txt)= ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad

# Flags combinés
echo "42 is nice" | ./ft_ssl md5 -p -r -s "foo" file.txt
```

---

## 🏗️ Architecture

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

## 📂 Structure du projet

<details>
<summary><b>Voir l'arborescence complète</b></summary>

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

</details>

---

## ⚙️ Contraintes techniques

- ✅ Langage **C uniquement**
- ✅ Fonctions autorisées : `open` `close` `read` `write` `malloc` `free`
- ❌ Interdits : `printf` `sprintf` `memcpy` `strlen` `strcmp` et toute libc
- ⚡ Pas de `if/else` chaîné pour le dispatch → **pointeurs de fonctions obligatoires**

---

## 🔨 Compilation

```bash
make        # compiler ft_ssl
make test   # lancer tous les tests unitaires
make re     # recompiler depuis zéro
make clean  # supprimer les objets
make fclean # supprimer les objets et ft_ssl
```

---

## ✅ Tests

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

## 🔐 Algorithmes

<table>
<tr>
<td width="50%">

### MD5 (RFC 1321)

- 🔢 Hash de **128 bits** (32 chars hex)
- 🧮 4 registres 32 bits (A B C D)
- 🔄 64 opérations en 4 rounds de 16
- 📊 **Little-endian**
- 📦 Padding : `0x80` + zéros jusqu'à 56 octets + taille 8 octets

**Documentation:**
- [docs/md5.md](docs/md5.md)
- [docs/transformation_md5.md](docs/transformation_md5.md)
- [docs/padding_md5.md](docs/padding_md5.md)

</td>
<td width="50%">

### SHA-256 (RFC 6234)

- 🔢 Hash de **256 bits** (64 chars hex)
- 🧮 8 registres 32 bits (H0..H7)
- 🔄 64 opérations avec message schedule étendu (16 → 64 mots)
- 📊 **Big-endian**
- 📦 Padding : `0x80` + zéros jusqu'à 56 octets + taille 8 octets

**Documentation:**
- [docs/sha256.md](docs/sha256.md)
- [docs/sha256_transform.md](docs/sha256_transform.md)
- [docs/sha256_padding.md](docs/sha256_padding.md)

</td>
</tr>
</table>

---

## 📚 Documentation par module

| Module | Fichier | Description |
|--------|---------|-------------|
| Parsing | [docs/parsing.md](docs/parsing.md) | parse_args, flags, fichiers, strings |
| Sérialisation | [docs/Sérialisation.md](docs/Sérialisation.md) | serialize_bytes, t_serial |
| Dispatch | [docs/dispatch.md](docs/dispatch.md) | t_hasher[], dispatch(), scalabilité |
| Run Hash | [docs/run_hash.md](docs/run_hash.md) | orchestration stdin/strings/fichiers |
| Display Hash | [docs/display_hash.md](docs/display_hash.md) | formatage -p -q -r |
| MD5 Transform | [docs/transformation_md5.md](docs/transformation_md5.md) | 64 opérations, macros |
| MD5 Padding | [docs/padding_md5.md](docs/padding_md5.md) | little-endian, cas limites |
| SHA-256 Init | [docs/sh256_init.md](docs/sh256_init.md) | valeurs RFC, H0..H7 |
| SHA-256 Process | [docs/sha256_process.md](docs/sha256_process.md) | streaming API, msg_size |
| Erreurs | [docs/erreurs_contraintes.md](docs/erreurs_contraintes.md) | 12 bugs documentés |

---

## 🐛 Bugs résolus

<details>
<summary><b>Voir les 12 bugs documentés</b></summary>

| # | Module | Bug | Solution |
|---|--------|-----|----------|
| 1 | ft_strcmp | Comparaison incorrecte après `\0` | Vérification stricte de fin de chaîne |
| 2 | ft_realloc | Arguments src/dst inversés | Correction de l'ordre des paramètres |
| 3 | compute_md5 | md5_init appelé après serialize | Réorganisation du pipeline |
| 4 | compute_md5 | Messages > 64 octets tronqués | Boucle de traitement par blocs |
| 5 | display_hash | Label stdin pour toutes les entrées en mode quiet | Gestion contextuelle du label |
| 6 | write_label | `\n` affiché dans le label `-p` | Filtrage du caractère de fin |
| 7 | ft_calloc | Typo `sstatic` | Correction syntaxe |
| 8 | headers | `#include <stdio.h>` interdit | Suppression dépendance libc |
| 9 | algorithme.h | Conflit de nom `MD5_H` | Renommage macro |
| 10 | compute_md5 | Compound literal → comportement indéfini | Allocation explicite |
| 11 | tests | `ctx` non initialisé → valeurs aléatoires | Initialisation systématique |
| 12 | openssl | `openssl md5 -s` cassé sur certaines versions | Gestion compatibilité |

→ [docs/erreurs_contraintes.md](docs/erreurs_contraintes.md)

</details>

---

## 🔍 Attack Surface Analysis

Ce projet explore les vulnérabilités cryptographiques des algorithmes de hachage :

### 🎯 Timing Attacks
- Comparaisons non-constant-time dans le code
- Possibilité de leak d'information via mesure du temps d'exécution

### 🔗 Length Extension
- Structure Merkle-Damgård exploitable sur MD5/SHA-256
- Permet d'étendre un hash sans connaître le message original

### 📦 Padding Oracle
- Mauvaise gestion du padding → information leak
- Exploitation possible sur messages malformés

### 💥 Collisions
- MD5 est **cryptographiquement cassé** (attaques SHAttered, Flame malware)
- SHA-256 reste sécurisé pour l'instant

---

## 👤 Auteur

**Mohamed Ouahab** ([@moouahab](https://github.com/moouahab))  
Projet École 42 Paris — ft_ssl [md5] [sha256]

---

<p align="center">
  <img src="https://img.shields.io/badge/Made_with-C-00599C?style=for-the-badge&logo=c"/>
  <img src="https://img.shields.io/badge/School-42_Paris-000000?style=for-the-badge&logo=42"/>
  <img src="https://img.shields.io/badge/Status-Completed-success?style=for-the-badge"/>
</p>