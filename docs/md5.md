# Comprendre MD5 — De zéro à l'implémentation

---

## Qu'est-ce que MD5 ?

MD5 (Message Digest 5) est un algorithme qui transforme n'importe quel
message en une empreinte unique de 32 caractères hexadécimaux.

```
"hello"          →  5d41402abc4b2a76b9719d911017c592
"hello!"         →  9b56e0a563ca8b5f9e03c5ac7e5e1a4b
"un fichier ISO" →  [32 caractères complètement différents]
```

Trois propriétés fondamentales :
- **Déterministe** — même entrée → toujours même sortie
- **Irréversible** — impossible de retrouver le message depuis le hash
- **Effet avalanche** — un seul bit changé → hash complètement différent

---

## Les briques de base — opérations sur les bits

MD5 travaille exclusivement sur des bits. Voici les opérations utilisées.

### ET logique (&)
Retourne 1 uniquement si les deux bits valent 1.
```
  1010
& 1100
------
  1000
```

### OU logique (|)
Retourne 1 si au moins un bit vaut 1.
```
  1010
| 1100
------
  1110
```

### XOR (^)
Retourne 1 si les bits sont différents.
```
  1010
^ 1100
------
  0110
```

### NOT (~)
Inverse chaque bit.
```
~ 1010
------
  0101
```

### Rotation de bits — ROTATE_LEFT(x, n)
Décale les bits vers la gauche. Les bits qui "sortent" reviennent à droite.
```
x      = 10110000 00000000 00000000 00000001
n      = 7

résultat : 00000000 00000000 00000000 11011000
                                      ↑
                              bits revenus par la droite
```

La rotation est différente du décalage (`<<`) — aucun bit n'est perdu.

---

## L'endianness — l'ordre des octets en mémoire

MD5 utilise le **little-endian** — l'octet le moins significatif est stocké
en premier.

```
valeur 32 bits : 0x12345678

big-endian    : [12][34][56][78]
little-endian : [78][56][34][12]   ← MD5 utilise cet ordre
```

Sur les processeurs x86 (Intel, AMD), la mémoire est naturellement
little-endian — MD5 tire parti de cela via les unions pour lire
les mots directement sans conversion.

---

## Étape 1 — Préparer le message : le padding

MD5 traite le message par blocs de **64 octets exactement**.
Si le message n'est pas un multiple de 64, on le complète.

### Les règles du padding

```
message original : [données]

après padding :
[données][0x80][0x00 ... 0x00][taille en bits sur 8 octets]
          ↑                    ↑
    marqueur de fin      toujours en positions 56 à 63
```

### Exemple avec "abc" (3 octets)

```
position  0 : 0x61  ('a')
position  1 : 0x62  ('b')
position  2 : 0x63  ('c')
position  3 : 0x80  ← marqueur obligatoire
position  4 : 0x00  ←
position  5 : 0x00    zéros de remplissage
...                 ←
position 55 : 0x00  ←
position 56 : 0x18  ← taille = 24 bits (3 × 8) en little-endian
position 57 : 0x00
...
position 63 : 0x00
```

### Cas particulier — message trop long pour le padding

Si le message dépasse 55 octets dans le dernier bloc, le padding ne tient
pas dans un seul bloc. On crée alors un deuxième bloc :

```
bloc 1 : [données][0x80][0x00 ... 0x00]   ← traité immédiatement
bloc 2 : [0x00 ... 0x00][taille]          ← bloc de padding pur
```

---

## Étape 2 — Découper en mots

Chaque bloc de 64 octets est vu comme **16 mots de 32 bits** grâce à l'union :

```c
typedef union {
    uint8_t     bytes[64];   // vue octet par octet
    uint32_t    words[16];   // vue mot par mot
} t_block32;
```

```
bloc : [61][62][63][80][00]...[18][00]...[00]
         ↓                      ↓
     words[0]               words[14]
   = 0x80636261            = 0x00000018
```

Les mots sont lus en little-endian automatiquement sur x86 — pas de
conversion nécessaire.

---

## Étape 3 — Les registres d'état

MD5 maintient 4 registres de 32 bits qui représentent l'état courant
du hachage. Ils sont initialisés aux valeurs définies par la RFC 1321 :

```
A = 0x67452301
B = 0xEFCDAB89
C = 0x98BADCFE
D = 0x10325476
```

Ces valeurs ne sont pas arbitraires — lues en little-endian octet par octet,
elles forment la séquence `01 23 45 67 89 AB CD EF FE DC BA 98 76 54 32 10`.

À chaque bloc traité, ces registres sont transformés et gardent l'état
cumulé de tout le message traité jusqu'ici.

---

## Étape 4 — La transformation : 64 opérations en 4 rounds

### Une opération élémentaire

Chaque opération suit ce schéma :

```
a = b + ROTATE_LEFT(a + f(b,c,d) + M[i] + T[i], s)
```

Décomposé :

```
f(b,c,d)        → fonction logique du round (F, G, H ou I)
M[i]            → mot du message courant
T[i]            → constante fixe (dérivée de sin)
s               → nombre de bits de rotation (entre 4 et 23)
ROTATE_LEFT     → rotation du résultat intermédiaire
+ b             → addition avec le registre b
→ nouveau a     → remplace l'ancienne valeur de a
```

Les registres sont ensuite permutés : A←D, D←C, C←B, B←nouveau A.

### Les 4 fonctions logiques

Chaque round utilise une fonction différente sur B, C, D :

```
Round 1 — F(B,C,D) = (B & C) | (~B & D)
          "Si B=1 prends C, sinon prends D"

Round 2 — G(B,C,D) = (B & D) | (C & ~D)
          "Si D=1 prends B, sinon prends C"

Round 3 — H(B,C,D) = B ^ C ^ D
          "XOR des trois registres"

Round 4 — I(B,C,D) = C ^ (B | ~D)
          "Mélange avec complément de D"
```

Chaque fonction a des propriétés bitwise différentes qui garantissent
que chaque bit d'entrée influence la sortie de manière distincte.

### Les constantes T[]

64 constantes fixes, dérivées de la fonction sinus :

```
T[i] = floor(2^32 × |sin(i + 1)|)
```

```
T[0]  = 0xd76aa478
T[1]  = 0xe8c7b756
...
T[63] = 0xeb86d391
```

Pourquoi le sinus ? Ces valeurs ont d'excellentes propriétés bitwise —
elles évitent les biais et renforcent la résistance de l'algorithme.

### Les décalages S[]

Chaque round a ses propres décalages de rotation, répétés 4 fois :

```
Round 1 :  7, 12, 17, 22
Round 2 :  5,  9, 14, 20
Round 3 :  4, 11, 16, 23
Round 4 :  6, 10, 15, 21
```

### L'ordre des mots par round

Les mots M[] ne sont pas toujours lus dans l'ordre — chaque round
utilise un ordre différent pour maximiser le mélange :

```
Round 1 : M[0], M[1], M[2], ..., M[15]         (ordre naturel)
Round 2 : M[1], M[6], M[11], M[0], ...         (formule : (5i+1) % 16)
Round 3 : M[5], M[8], M[11], M[14], ...        (formule : (3i+5) % 16)
Round 4 : M[0], M[7], M[14], M[5], ...         (formule : (7i)   % 16)
```

---

## Étape 5 — L'addition finale

Après les 64 opérations, les registres transformés sont additionnés
aux valeurs du début du bloc :

```
A_final = A_initial + A_transformé   (overflow uint32_t volontaire)
B_final = B_initial + B_transformé
C_final = C_initial + C_transformé
D_final = D_initial + D_transformé
```

Pourquoi ? Cette addition crée une dépendance entre les blocs —
l'état après chaque bloc influence le traitement du bloc suivant.
C'est ce qui rend le hash sensible à l'ordre et au contenu de
chaque bloc.

---

## Étape 6 — Produire le hash final

Après tous les blocs, les 4 registres contiennent le hash.
On les convertit en hexadécimal octet par octet en little-endian :

```
A = 0x98500190
octets little-endian : [90][01][50][98]
hex string           : "90015098"

B = 0xb04fd23c → "3cd24fb0"
C = 0x7d3f96d6 → "d6963f7d"
D = 0x727fe128 → "28e17f72"

hash final : "900150983cd24fb0d6963f7d28e17f72"
```

---

## Validation — MD5("abc")

Voici le déroulement complet pour "abc" :

```
1. Padding
   bytes[0..2] = 'a' 'b' 'c'
   bytes[3]    = 0x80
   bytes[4..55]= 0x00
   bytes[56]   = 0x18  (24 bits)

2. Sérialisation
   words[0]  = 0x80636261
   words[14] = 0x00000018

3. Initialisation
   A = 0x67452301
   B = 0xEFCDAB89
   C = 0x98BADCFE
   D = 0x10325476

4. Après 64 opérations + addition
   A = 0x98500190
   B = 0xb04fd23c
   C = 0x7d3f96d6
   D = 0x727fe128

5. Hash final
   "900150983cd24fb0d6963f7d28e17f72"
```

---

## Schéma global

```
message (taille quelconque)
         ↓
    [  Padding  ]  → multiple de 64 octets garanti
         ↓
    [  Bloc 1  ]  → sérialisation → 16 mots de 32 bits
         ↓
    [  Bloc 1  ]  → 64 opérations (4 rounds × 16 ops)
         ↓
    A B C D mis à jour
         ↓
    [  Bloc 2  ]  → même traitement
         ↓
    ...
         ↓
    [  Bloc N  ]  → dernier bloc
         ↓
    A B C D → conversion hex little-endian
         ↓
    hash de 32 caractères
```

---

## Fichiers du module

```
src/module/md5/
├── md5_init.c       → initialise A B C D et le bloc à zéro
├── md5_padding.c    → ajoute 0x80, zéros et taille
├── md5_process.c    → appelle transform puis efface le bloc
├── md5_transform.c  → les 64 opérations en 4 rounds
└── md5_final.c      → orchestre tout et produit le hash hex

src/include/
├── algorithme.h     → macros MD5_A/B/C/D, MD5_F/G/XOR/I,
│                      MD5_ROTATE, MD5_OP
└── md5.h            → déclarations des fonctions publiques
```