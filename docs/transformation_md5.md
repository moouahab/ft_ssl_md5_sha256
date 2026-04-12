# MD5 — La Transformation

---

## C'est quoi MD5 en vrai ?

Imagine que tu as un texte — par exemple "hello". MD5 va le transformer
en une suite de 32 caractères incompréhensibles :

```
"hello" → 5d41402abc4b2a76b9719d911017c592
```

Ce résultat s'appelle un **hash**. Il a des propriétés magiques :
- Toujours la même sortie pour la même entrée
- Impossible de retrouver "hello" depuis le hash
- Un seul caractère changé → hash complètement différent

---

## Comment ça marche en gros ?

Pense à MD5 comme une machine à mélanger. Tu mets des ingrédients
(ton message), tu tournes la manivelle 64 fois, et tu obtiens
toujours le même gâteau pour les mêmes ingrédients.

```
message  ──────────────────────────────────→  hash
"hello"  → [machine MD5 : 64 tours]  →  5d41402abc...
```

---

## Les 4 tiroirs — les registres A B C D

MD5 utilise 4 variables de travail qu'on appelle des **registres**.
Pense à eux comme 4 tiroirs qui contiennent des nombres.

```
Tiroir A : 0x67452301
Tiroir B : 0xEFCDAB89
Tiroir C : 0x98BADCFE
Tiroir D : 0x10325476
```

Ces valeurs de départ sont fixées par le standard MD5 — tout le monde
commence avec les mêmes valeurs. À la fin, ces 4 tiroirs contiennent
le hash.

---

## Les blocs — découper le message

MD5 ne traite pas tout le message d'un coup. Il le découpe en
**morceaux de 64 octets** (512 bits).

```
message long :
[bloc 1 : 64 octets][bloc 2 : 64 octets][bloc 3 : 64 octets]
       ↓                    ↓                    ↓
   64 opérations        64 opérations        64 opérations
```

Chaque bloc est découpé en 16 mots de 32 bits :

```
bloc de 64 octets
[mot 0][mot 1][mot 2]...[mot 15]
   ↑
4 octets = 32 bits = un mot
```

---

## Le padding — rembourrage du dernier bloc

Si le message ne remplit pas exactement 64 octets, on complète.

```
"abc" = 3 octets → trop court pour un bloc de 64

[a][b][c][0x80][0x00]...[0x00][taille du message]
                               ↑
                         8 derniers octets
```

Les règles :
1. On ajoute un octet `0x80` juste après le message
2. On remplit de `0x00` jusqu'à 56 octets
3. On écrit la taille originale en bits dans les 8 derniers octets

---

## Les opérations bitwise — le langage des bits

Avant de comprendre les opérations MD5, il faut comprendre
les opérations sur les bits.

### AND (&) — le ET logique
```
  1010
& 1100
------
  1000   ← 1 seulement si les deux bits sont 1
```

### OR (|) — le OU logique
```
  1010
| 1100
------
  1110   ← 1 si au moins un bit est 1
```

### XOR (^) — le OU exclusif
```
  1010
^ 1100
------
  0110   ← 1 si les bits sont différents
```

### NOT (~) — l'inverse
```
~ 1010
------
  0101   ← chaque bit est inversé
```

### Rotation de bits — ROTATE_LEFT
```
nombre : 10110000 00000000 00000000 00000001
rotation de 7 bits vers la gauche :

les 7 bits qui "sortent" à gauche → reviennent à droite
résultat : 00000000 00000000 00000000 11011000
                                      ↑
                              les bits qui sont revenus
```

La rotation est différente du décalage (<<) car elle ne perd aucun bit.

---

## Les 4 fonctions — le secret de chaque round

MD5 utilise 4 "recettes" différentes pour mélanger les registres B, C, D.

### Fonction F — Round 1
```
F(B, C, D) = (B ET C) OU (NON-B ET D)
```

En français : "pour chaque bit, si B vaut 1 → prends C, sinon prends D"

```
B = 1010
C = 1100
D = 0011

F = (1010 ET 1100) OU (0101 ET 0011)
  = (1000)          OU (0001)
  =  1001
```

### Fonction G — Round 2
```
G(B, C, D) = (B ET D) OU (C ET NON-D)
```

L'inverse de F — "si D vaut 1 → prends B, sinon prends C"

### Fonction H — Round 3
```
H(B, C, D) = B XOR C XOR D
```

Le plus simple — chaque bit = XOR des 3 registres

### Fonction I — Round 4
```
I(B, C, D) = C XOR (B OU NON-D)
```

Le plus complexe — mélange avec complément

---

## Une opération MD5 — décomposée

Voici ce qui se passe à chaque étape d'un round :

```
Étape 1 — calculer la fonction logique
    f = F(B, C, D)   (ou G, H, I selon le round)

Étape 2 — additionner 4 valeurs
    temp = A + f + M[i] + T[i]
    ↑       ↑    ↑       ↑
    registre|    |       constante fixe
            |    mot du message
            résultat de la fonction

Étape 3 — rotation
    temp = ROTATE_LEFT(temp, s)
    (s varie entre 4 et 23 selon la position)

Étape 4 — addition avec B
    temp = B + temp

Étape 5 — rotation des registres
    A = D
    D = C
    C = B
    B = temp
```

---

## Les 64 constantes T[] — d'où viennent-elles ?

Chaque opération utilise une constante T[i] différente.
Ces constantes sont dérivées de la fonction mathématique sinus :

```
T[i] = partie entière de (2^32 × |sin(i + 1)|)
```

En pratique, on les copie directement depuis la RFC 1321 :

```c
T[0]  = 0xd76aa478   (sin(1)  × 2^32)
T[1]  = 0xe8c7b756   (sin(2)  × 2^32)
T[2]  = 0x242070db   (sin(3)  × 2^32)
// ... 61 autres valeurs
```

Pourquoi le sinus ? Ces valeurs ont de bonnes propriétés bitwise —
elles évitent les biais dans les bits et renforcent la "confusion"
dans l'algorithme.

---

## Les 4 rounds — vue d'ensemble

### Round 1 : 16 opérations avec F
```
Mot 0, Mot 1, Mot 2 ... Mot 15 (dans l'ordre)
Rotation : 7, 12, 17, 22 (répété 4 fois)
```

### Round 2 : 16 opérations avec G
```
Mots dans un ordre différent : 1, 6, 11, 0, 5, 10, 15, 4...
Rotation : 5, 9, 14, 20 (répété 4 fois)
```

### Round 3 : 16 opérations avec H
```
Encore un autre ordre : 5, 8, 11, 14, 1, 4, 7, 10...
Rotation : 4, 11, 16, 23 (répété 4 fois)
```

### Round 4 : 16 opérations avec I
```
Dernier ordre : 0, 7, 14, 5, 12, 3, 10, 1...
Rotation : 6, 10, 15, 21 (répété 4 fois)
```

Pourquoi des ordres différents ? Pour que chaque mot du message
influence les registres de plusieurs façons différentes.

---

## L'addition finale — pourquoi ?

Après les 64 opérations, on additionne les registres transformés
aux valeurs initiales :

```
avant transform : A=0x67452301  B=0xEFCDAB89  C=0x98BADCFE  D=0x10325476
après 64 ops   : A=0x3113ef8f  B=0x6478a689  C=0xfffbeaf8  D=0x247568b2

addition finale :
A = 0x67452301 + 0x3113ef8f = 0x98500190
B = 0xEFCDAB89 + 0x6478a689 = 0xb04fd23c (overflow volontaire)
C = 0x98BADCFE + 0xfffbeaf8 = 0x7d3f96d6 (overflow volontaire)
D = 0x10325476 + 0x247568b2 = 0x34a7bd28 ... hmm
```

Cette addition crée une dépendance avec le bloc précédent —
chaque bloc est influencé par tous les blocs qui le précèdent.

---

## Le hash final — convertir en hexadécimal

Après tous les blocs, les registres A B C D contiennent le hash.
On les convertit en hexadécimal en **little-endian** :

```
A = 0x98500190
En octets little-endian : 90 01 50 98
En hex string           : "90015098"

B = 0xb04fd23c
En octets little-endian : 3c d2 4f b0
En hex string           : "3cd24fb0"

C = 0x7d3f96d6 → "d6963f7d"
D = 0x727fe128 → "28e17f72"

Hash final : "900150983cd24fb0d6963f7d28e17f72"
```

---

## Little-endian — qu'est-ce que c'est ?

C'est l'ordre dans lequel les octets sont stockés en mémoire.

```
valeur : 0x12345678

big-endian    : [12][34][56][78]  (octet le plus significatif en premier)
little-endian : [78][56][34][12]  (octet le moins significatif en premier)
```

MD5 utilise le little-endian — c'est pourquoi les processeurs x86
(Intel, AMD) peuvent lire les mots directement sans conversion.

---

## Résumé visuel complet

```
message "abc"
      ↓
padding : [a][b][c][0x80][0x00...][taille]
      ↓
16 mots de 32 bits : M[0]=0x80636261 ... M[14]=24 M[15]=0
      ↓
md5_init : A=0x67452301 B=0xEFCDAB89 C=0x98BADCFE D=0x10325476
      ↓
Round 1 (16 ops avec F) :  mélange les registres avec les mots
      ↓
Round 2 (16 ops avec G) :  mélange différemment
      ↓
Round 3 (16 ops avec H) :  encore différemment
      ↓
Round 4 (16 ops avec I) :  dernière couche de mélange
      ↓
addition : A+=a B+=b C+=c D+=d
      ↓
A=0x98500190 B=0xb04fd23c C=0x7d3f96d6 D=0x727fe128
      ↓
conversion little-endian en hex
      ↓
"900150983cd24fb0d6963f7d28e17f72"
```