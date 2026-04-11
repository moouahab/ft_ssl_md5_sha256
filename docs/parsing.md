# Documentation — Module de Parsing `ft_ssl`

---

## Vue d'ensemble

Le module de parsing transforme les arguments `argv` en une structure `t_args` exploitable par les algorithmes de hachage. Il gère les commandes, les flags, les strings et les fichiers dans n'importe quel ordre.

---

## Structures de données

### `t_flags`
```c
typedef struct s_flags
{
    bool    p;   // echo STDIN + checksum
    bool    q;   // mode silencieux
    bool    r;   // inverse le format de sortie
    bool    s;   // hash d'une string
}   t_flags;
```

### `t_file`
```c
typedef struct s_file
{
    char    *name;   // nom du fichier (pointe vers argv)
    int     fd;      // descripteur (-1 = pas encore ouvert)
}   t_file;
```

### `t_args`
```c
typedef struct s_args
{
    t_flags     flags;              // état des 4 flags
    char        *command;           // "md5" ou "sha256"
    t_file      *files;             // tableau dynamique de fichiers
    int         files_count;        // nombre de fichiers
    int         files_capacity;     // capacité allouée
    char        **strings;          // tableau dynamique de strings (-s)
    int         strings_count;      // nombre de strings
    int         strings_capacity;   // capacité allouée
}   t_args;
```

---

## Règles de parsing

### Ordre des arguments
```
./ft_ssl <commande> [flags/-s "str"] [fichiers]
```

- `argv[0]` → nom du binaire (`ft_ssl`)
- `argv[1]` → commande (`md5` ou `sha256`)
- `argv[2+]` → flags, strings, fichiers dans n'importe quel ordre

### Règle critique — état `files_started`
```
Avant le premier fichier  → flags et -s acceptés
Dès qu'un fichier apparaît → tout ce qui suit est un fichier
                             même si ça commence par '-'
```

Exemple :
```bash
./ft_ssl md5 -r -p -s "foo" file -s "bar"
#                            ↑
#                     files_started = true
#                     '-s' et 'bar' → traités comme fichiers → erreur
```

---

## Fonctions

### `check_command`
```c
bool check_command(t_args *args, char *command);
```
Valide que la commande est `md5` ou `sha256` et la stocke dans `args->command`.

| Retour | Signification |
|--------|--------------|
| `true` | commande valide, stockée |
| `false` | commande invalide |

---

### `check_flag`
```c
bool check_flag(t_args *args, char *flag);
```
Active le booléen correspondant dans `args->flags`.

| Flag | Action |
|------|--------|
| `-p` | `args->flags.p = true` |
| `-q` | `args->flags.q = true` |
| `-r` | `args->flags.r = true` |
| `-s` | `args->flags.s = true` (la string est consommée dans `parse_flags`) |

> ⚠️ `-s` active seulement le booléen ici. La consommation de l'argument suivant se fait dans `parse_flags`.

---

### `add_file`
```c
bool add_file(t_args *args, char *filename);
```
Ajoute un fichier au tableau dynamique `args->files`.

- Double la capacité si nécessaire via `ft_realloc`
- Initialise `fd = -1` (ouverture différée)
- Retourne `false` si `ft_realloc` échoue

---

### `add_string`
```c
bool add_string(t_args *args, char *string);
```
Ajoute une string au tableau dynamique `args->strings`.

- Même logique que `add_file`
- Les pointeurs pointent vers `argv` — ne pas `free` individuellement

---

### `parse_flags`
```c
static bool parse_flags(t_args *args, char **av, int *i);
```
Boucle principale sur `argv[2+]` :

```
av[i][0] == '-' && !files_started
    → check_flag
    → si -s : i++ et add_string(av[i])

sinon
    → files_started = true
    → add_file(av[i])
```

---

### `parse_args`
```c
t_args *parse_args(int ac, char **av);
```
Point d'entrée du parsing. Orchestre tout :

```
ac < 2          → affiche usage → NULL
check_command   → commande invalide → affiche erreur → NULL
parse_flags     → erreur → NULL
succès          → retourne t_args*
```

---

### `ft_free_args`
```c
void ft_free_args(t_args *args);
```
Libère toute la mémoire allouée :

```
files[]     → ferme les fd ouverts (fd != -1) → free tableau
strings[]   → free tableau seulement (pointeurs vers argv)
args        → free la structure
```

> ⚠️ Ne jamais `free` les strings individuelles ni les `name` de fichiers — ils pointent vers `argv`.

---

## Exemples de comportement

```bash
# Usage
./ft_ssl
→ usage: ft_ssl command [flags] [file/string]

# Commande invalide
./ft_ssl foobar
→ ft_ssl: Error: 'foobar' is an invalid command.
→ Commands: md5 sha256 | Flags: -p -q -r -s

# Flags simples
./ft_ssl md5 -p -q -r
→ command="md5" flags={p,q,r}

# Avec string
./ft_ssl md5 -s "hello"
→ command="md5" strings=["hello"]

# Combinaison complète
./ft_ssl md5 -p -r -s "foo" file.txt
→ command="md5" flags={p,r} strings=["foo"] files=["file.txt"]

# Cas piège — après fichier tout est fichier
./ft_ssl md5 -r -p -s "foo" file -s "bar"
→ strings=["foo"] files=["file", "-s", "bar"]
→ erreur à l'ouverture de "-s" et "bar"
```

---

## Gestion mémoire

| Pointeur | Alloué par | Libéré par |
|----------|-----------|-----------|
| `args` | `ft_calloc` | `ft_free_args` |
| `args->files` | `ft_realloc` | `ft_free_args` |
| `args->strings` | `ft_realloc` | `ft_free_args` |
| `args->files[i].name` | `argv` (système) | jamais |
| `args->strings[i]` | `argv` (système) | jamais |

---

## Fichiers du module

```
src/
└── module/
    └── args/
        ├── check_args.c    → check_command, check_flag
        ├── parse_args.c    → parse_flags, parse_args
        └── free_args.c     → ft_free_args
src/include/
    └── args.h              → déclarations + structures
```