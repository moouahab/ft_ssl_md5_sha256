# Documentation — Module Display Hash

---

## Rôle

`display_hash` formate et affiche le résultat d'un hash selon les flags
activés. Il est générique — il fonctionne pour tous les algorithmes
via `t_display`.

---

## Séparation stdin / strings+fichiers

Deux fonctions distinctes gèrent l'affichage :

```
display_stdin_hash()  → appelé par handle_stdin dans run_hash.c
                        gère tous les cas stdin

display_hash()        → appelé par handle_strings et handle_files
                        gère strings et fichiers uniquement
```

Cette séparation évite que `display_hash` ait à distinguer
l'origine de la donnée — il reçoit toujours un label non-nul.

---

## `display_hash`

```c
void    display_hash(t_display *disp, char *hash,
                     char *label, bool is_file)
```

### Logique

```
disp NULL ou hash NULL  → return
quiet                   → hash\n
label NULL              → return (stdin géré ailleurs)
reverse + fichier       → hash label\n
reverse + string        → hash "label"\n
fichier défaut          → MD5(label)= hash\n
string défaut           → MD5("label")= hash\n
```

### Exemples

```bash
# défaut fichier
MD5(/tmp/file)= 53d53ea94217b259c11a5a2d104ec58a

# défaut string
MD5("foo")= acbd18db4cc2f85cedef654fccc4a4d8

# -r fichier
53d53ea94217b259c11a5a2d104ec58a /tmp/file

# -r string
acbd18db4cc2f85cedef654fccc4a4d8 "foo"

# -q (tous les cas)
acbd18db4cc2f85cedef654fccc4a4d8
```

---

## Fonctions statiques

### `print_label_file`

```
MD5(label)= hash
```

```c
static void     print_label_file(t_display *disp, char *hash, char *label)
```

### `print_label_string`

```
MD5("label")= hash
```

```c
static void     print_label_string(t_display *disp, char *hash, char *label)
```

### `print_reverse_file`

```
hash label
```

```c
static void     print_reverse_file(char *hash, char *label)
```

### `print_reverse_string`

```
hash "label"
```

```c
static void     print_reverse_string(char *hash, char *label)
```

---

## `display_stdin_hash`

```c
static void     display_stdin_hash(t_display *disp,
                                    char *hash, char *label)
```

### Tableau des formats

| `-p` | `-q` | `-r` | Format |
|------|------|------|--------|
| non  | non  | non  | `MD5(stdin)= hash` |
| oui  | non  | non  | `("label")= hash` |
| non  | oui  | non  | `hash` |
| oui  | oui  | non  | `label\nhash` |

Note : `-r` est ignoré pour stdin — le format `("label")= hash`
est toujours utilisé avec `-p`, peu importe `-r`.

### Exemples

```bash
# stdin normal
echo -n "abc" | ./ft_ssl md5
MD5(stdin)= 900150983cd24fb0d6963f7d28e17f72

# stdin avec -p
echo -n "abc" | ./ft_ssl md5 -p
("abc")= 900150983cd24fb0d6963f7d28e17f72

# stdin avec newline + -p
echo "42 is nice" | ./ft_ssl md5 -p
("42 is nice")= 35f1d6de0302e2086a4e472266efb3a9

# stdin quiet
echo -n "abc" | ./ft_ssl md5 -q
900150983cd24fb0d6963f7d28e17f72

# stdin quiet + -p
echo "just to be extra clear" | ./ft_ssl md5 -q -p
just to be extra clear
3ba35f1ea0d170cb3b9a752e3360286c
```

---

## Formatage du label stdin — `write_label`

Le label affiché avec `-p` est le contenu brut de stdin transformé :

| Octet | Affiché |
|-------|---------|
| `\n` final | supprimé |
| `\n` interne | `\\n` |
| `\t` | `\\t` |
| autres | inchangé |

**Pourquoi supprimer le `\n` final ?**

`echo "foo"` ajoute un `\n` — sans suppression le label serait
`("foo\n")=` au lieu de `("foo")=`. Le hash inclut bien ce `\n`
mais le label l'affiche sans.

---

## Protections

```
disp NULL              → return (display_hash)
hash NULL              → return (display_hash)
disp->hasher NULL      → return (display_hash)
disp->hasher->label NULL → return (display_hash)
label NULL             → return (display_hash, stdin géré ailleurs)
```

---

## Fichiers

```
src/module/algohash/
└── display_hash.c   → display_hash + display_stdin_hash
                       + print_label_file/string
                       + print_reverse_file/string

src/include/
└── hash.h           → déclaration display_hash()
```
