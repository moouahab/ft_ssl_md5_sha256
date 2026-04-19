#ifndef HASH_H
# define HASH_H

# include <stdbool.h>
# include <fcntl.h>
# include "args.h"
# include "input.h"
# include "serialize.h"
# include "md5.h"

// type du pointeur de fonction de hash
typedef char            *(*t_hash_fn)(t_input *);

// associe un algo à sa fonction et son label
typedef struct  s_hasher
{
    char        *name;      // "md5" → utilisé pour le dispatch
    char        *label;     // "MD5" → utilisé pour l'affichage
    t_hash_fn   hash;       // compute_md5 → fonction de calcul
}               t_hasher;

// paramètres d'affichage
typedef struct  s_display
{
    t_hasher    *hasher;
    bool        quiet;
    bool        reverse;
    bool        print_stdin;
    bool        is_stdin;
}               t_display;

// fonctions publiques
void    dispatch(t_args *args);
void    run_hash(t_args *args, t_hasher *hasher);
void    display_hash(t_display *disp, char *hash, char *label, bool is_file);

// fonctions de calcul
char    *compute_md5(t_input *input);
char    *compute_sha256(t_input *input);
char    *compute_sha512(t_input *input);
char    *compute_whirlpool(t_input *input);

bool    is_valid_command(char *command);
void    print_commands(void);

#endif