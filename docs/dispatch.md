# Documentation — Module Dispatch

---

## Rôle

Le dispatch est le point de connexion entre le parsing des arguments
et l'exécution du bon algorithme de hachage.

```
main()
  ↓
parse_args()  → t_args { command, flags, strings, files }
  ↓
dispatch()    → trouve compute_XXX via hashers[]
  ↓
run_hash()    → exécute le pipeline complet
```

---

## Pourquoi un tableau de structs

Le sujet interdit explicitement les chaînes de `if/else` pour le dispatch.
Un tableau de structs avec pointeurs de fonctions est la solution propre :

```c
// INTERDIT par le sujet
if (ft_strcmp(cmd, "md5") == 0)
    run_md5(args);
else if (ft_strcmp(cmd, "sha256") == 0)
    run_sha256(args);

// CORRECT
static const t_hasher   hashers[] = {
    { "md5",    "MD5",    compute_md5    },
    { "sha256", "SHA256", compute_sha256 },
    { NULL,     NULL,     NULL           }
};
```

---

## Structure `t_hasher`

```c
typedef char    *(*t_hash_fn)(t_input *);

typedef struct  s_hasher
{
    char        *name;      // "md5"    → comparé à args->command
    char        *label;     // "MD5"    → affiché dans le résultat
    t_hash_fn   hash;       // compute_md5 → appelé pour calculer
}               t_hasher;
```

Chaque entrée du tableau associe :
- un nom de commande reconnu par le parsing
- un label d'affichage
- une fonction de calcul avec signature uniforme

---

## Implémentation

```c
static const t_hasher   hashers[] = {
    { "md5",    "MD5",    compute_md5    },
    { "sha256", "SHA256", compute_sha256 },
    { NULL,     NULL,     NULL           }   // sentinelle de fin
};

void    dispatch(t_args *args)
{
    int     i;

    if (!args)
        return ;
    i = 0;
    while (hashers[i].name)
    {
        if (ft_strcmp(args->command, hashers[i].name) == 0)
        {
            run_hash(args, (t_hasher *)&hashers[i]);
            return ;
        }
        i++;
    }
}
```

### Pourquoi `static const`

```
static  → tableau local au fichier dispatch.c
          pas de conflit si un autre fichier définit aussi hashers[]
const   → tableau immuable → mis en .rodata
          le compilateur peut optimiser les accès
```

### Pourquoi la sentinelle `NULL`

```c
{ NULL, NULL, NULL }
```

Permet de parcourir le tableau sans connaître sa taille.
La boucle s'arrête naturellement à la première entrée nulle.

---

## Ajouter un algorithme

### Étape 1 — Coder la fonction de calcul

```c
// algo_whirlpool.c
char    *compute_whirlpool(t_input *input)
{
    // même structure que compute_md5
    // mais avec le contexte Whirlpool
}
```

### Étape 2 — Déclarer dans `hash.h`

```c
char    *compute_whirlpool(t_input *input);
```

### Étape 3 — Ajouter dans `hashers[]`

```c
static const t_hasher   hashers[] = {
    { "md5",       "MD5",       compute_md5       },
    { "sha256",    "SHA256",    compute_sha256     },
    { "whirlpool", "WHIRLPOOL", compute_whirlpool  },
    { NULL,        NULL,        NULL               }
};
```

### Étape 4 — Ajouter dans le message d'erreur

```c
// check_args.c
printf("Commands:\nmd5\nsha256\nwhirlpool\n");
```

---

## Ce que dispatch ne fait PAS

```
valider la commande  → fait dans parse_args ✓
gérer les erreurs    → parse_args retourne NULL ✓
afficher le hash     → délégué à run_hash ✓
```

---

## Fichiers

```
src/module/algohash/
└── dispatch.c      → hashers[] + dispatch()

src/include/
└── hash.h          → t_hash_fn, t_hasher, déclaration dispatch()
```
