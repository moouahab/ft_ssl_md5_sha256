# Documentation — Module Run Hash

---

## Rôle

`run_hash` orchestre le pipeline complet entre un `t_hasher` et
l'affichage final. Il est générique — il fonctionne pour MD5,
SHA-256, ou tout autre algorithme enregistré dans le dispatch.

---

## Flux d'exécution

```
run_hash(args, hasher)
    ↓
construire t_display
    ↓
flag -p ?  → handle_stdin
    ↓
strings[]  → handle_strings
    ↓
files[]    → handle_files
    ↓
rien + pas -p ? → handle_stdin (stdin par défaut)
```

---

## Fonctions

### `run_hash`

```c
void    run_hash(t_args *args, t_hasher *hasher)
```

Point d'entrée. Construit `t_display` puis appelle les handlers dans l'ordre.

**Protections :**
```
args NULL           → return
hasher NULL         → return
hasher->hash NULL   → return (fonction manquante)
hasher->name NULL   → return
hasher->label NULL  → return
```

---

### `handle_stdin`

```c
static void     handle_stdin(t_display *disp)
```

Lit STDIN, formate le label, affiche le résultat.

**Étapes :**
```
1. input_from_stdin()          → lire fd 0
2. disp->hasher->hash(input)   → calculer le hash
3. write_label()               → formater le contenu
4. display_stdin_hash()        → afficher selon les flags
5. free(hash) + free_input()   → libérer
```

**Cas `-p` :** le label = contenu lu (sans `\n` final)
**Cas sans `-p` :** le label = `NULL` → format `MD5(stdin)= hash`

---

### `handle_strings`

```c
static void     handle_strings(t_args *args, t_display *disp)
```

Parcourt `args->strings[]` et hache chaque string.

**Protections :**
```
args NULL               → return
strings_count < 0       → return
strings[i] NULL         → skip
input_from_string échoue → skip
hash échoue             → skip, free input
```

**Mémoire :**
```
free_input(input, false)  → ne pas free data (pointe vers argv)
free(hash)                → alloué par md5()
```

---

### `handle_files`

```c
static void     handle_files(t_args *args, t_display *disp)
```

Ouvre et hache chaque fichier de `args->files[]`.

**Protections :**
```
args NULL               → return
files_count < 0         → return
files[i].name NULL      → skip
open() échoue           → message erreur stderr → skip
input_from_fd échoue    → skip
hash échoue             → skip, free input
```

**Mémoire :**
```
close(fd) + fd = -1       → fermer après lecture
free_input(input, true)   → free data (alloué par ft_realloc)
free(hash)                → alloué par md5()
```

**Format erreur :**
```
ft_ssl: md5: /chemin: No such file or directory
```

---

### `write_label`

```c
static void     write_label(uint8_t *data, uint64_t size,
                             char *buf, int buf_size)
```

Formate le contenu stdin pour affichage avec `-p`.

**Transformations :**
```
\n final     → supprimé
\n interne   → \\n
\t           → \\t
autres       → copiés tels quels
```

**Exemples :**
```
"hello\n"     → "hello"
"foo\nbar\n"  → "foo\\nbar"
"a\tb"        → "a\\tb"
"abc"         → "abc"
```

---

### `display_stdin_hash`

```c
static void     display_stdin_hash(t_display *disp,
                                    char *hash, char *label)
```

Affiche le résultat stdin selon la combinaison de flags.

**Cas :**

| Flags | Affichage |
|-------|-----------|
| aucun | `MD5(stdin)= hash` |
| `-p` | `("label")= hash` |
| `-q` | `hash` |
| `-q -p` | `label\nhash` |

---

### `check_disp`

```c
static bool     check_disp(t_display *disp)
```

Valide que `t_display` est utilisable avant tout traitement.

```
disp NULL          → false
disp->hasher NULL  → false
hasher->hash NULL  → false
hasher->name NULL  → false
hasher->label NULL → false
sinon              → true
```

---

## Fichiers

```
src/module/algohash/
└── run_hash.c    → toutes les fonctions ci-dessus

src/include/
└── hash.h        → déclaration void run_hash(t_args *, t_hasher *)
```
