#!/bin/bash

# ================================================================
# test_ft_ssl.sh — Comparaison ft_ssl vs openssl (md5 + sha256)
# Usage : ./test_ft_ssl.sh [chemin vers ft_ssl]
# ================================================================

FT_SSL=${1:-"../ft_ssl"}
PASS=0
FAIL=0
TOTAL=0

GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'

# ================================================================
# helpers
# ================================================================

check()
{
    local label="$1"
    local ft_out="$2"
    local ssl_out="$3"

    TOTAL=$((TOTAL + 1))
    if [ "$ft_out" = "$ssl_out" ]; then
        echo -e "${GREEN}[OK]${RESET} $label"
        PASS=$((PASS + 1))
    else
        echo -e "${RED}[KO]${RESET} $label"
        diff <(echo "$ssl_out") <(echo "$ft_out") | sed 's/^/     /'
        FAIL=$((FAIL + 1))
    fi
}

check_manual()
{
    local label="$1"
    local expected="$2"
    local got="$3"

    TOTAL=$((TOTAL + 1))
    if [ "$expected" = "$got" ]; then
        echo -e "${GREEN}[OK]${RESET} $label"
        PASS=$((PASS + 1))
    else
        echo -e "${RED}[KO]${RESET} $label"
        diff <(echo "$expected") <(echo "$got") | sed 's/^/     /'
        FAIL=$((FAIL + 1))
    fi
}

setup()
{
    echo "And above all," > /tmp/ft_ssl_test_file
    echo "https://www.42.fr/" > /tmp/ft_ssl_test_website
    printf 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' \
        > /tmp/ft_ssl_test_57a
    printf 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' \
        > /tmp/ft_ssl_test_64a
}

cleanup()
{
    rm -f /tmp/ft_ssl_test_file
    rm -f /tmp/ft_ssl_test_website
    rm -f /tmp/ft_ssl_test_57a
    rm -f /tmp/ft_ssl_test_64a
}

# ================================================================
# SHA-256 STDIN — comparaison directe openssl vs ft_ssl
# ================================================================

test_sha256_stdin()
{
    echo "=== SHA256 STDIN ==="

    check "stdin abc" \
        "$(echo -n "abc" | $FT_SSL sha256)" \
        "$(echo -n "abc" | openssl sha256)"

    check "stdin vide" \
        "$(echo -n "" | $FT_SSL sha256)" \
        "$(echo -n "" | openssl sha256)"

    check "stdin avec newline" \
        "$(echo "42 is nice" | $FT_SSL sha256)" \
        "$(echo "42 is nice" | openssl sha256)"

    check "stdin 55 chars" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL sha256)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl sha256)"

    check "stdin 56 chars" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL sha256)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl sha256)"

    check "stdin 64 chars" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL sha256)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl sha256)"

    check "stdin 65 chars" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL sha256)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl sha256)"

    check "stdin long message" \
        "$(echo -n "The quick brown fox jumps over the lazy dog" | $FT_SSL sha256)" \
        "$(echo -n "The quick brown fox jumps over the lazy dog" | openssl sha256)"

    check "stdin pity..." \
        "$(echo -n "pity those that aren't following baerista on spotify." | $FT_SSL sha256)" \
        "$(echo -n "pity those that aren't following baerista on spotify." | openssl sha256)"
}

# ================================================================
# SHA-256 FLAG -s
# ================================================================

test_sha256_flag_s()
{
    echo ""
    echo "=== SHA256 FLAG -s ==="

    check "-s abc hash correct" \
        "$(echo -n "abc" | $FT_SSL sha256 -q)" \
        "$(echo -n "abc" | openssl sha256 | awk '{print $2}')"

    check "-s foo hash correct" \
        "$(echo -n "foo" | $FT_SSL sha256 -q)" \
        "$(echo -n "foo" | openssl sha256 | awk '{print $2}')"

    check "-s vide hash correct" \
        "$(echo -n "" | $FT_SSL sha256 -q)" \
        "$(echo -n "" | openssl sha256 | awk '{print $2}')"

    check "-s pity... hash correct" \
        "$(echo -n "pity those that aren't following baerista on spotify." | $FT_SSL sha256 -q)" \
        "$(echo -n "pity those that aren't following baerista on spotify." | openssl sha256 | awk '{print $2}')"
}

# ================================================================
# SHA-256 FLAG -q
# ================================================================

test_sha256_flag_q()
{
    echo ""
    echo "=== SHA256 FLAG -q ==="

    check "-q stdin abc" \
        "$(echo -n "abc" | $FT_SSL sha256 -q)" \
        "$(echo -n "abc" | openssl sha256 | awk '{print $2}')"

    check "-q stdin vide" \
        "$(echo -n "" | $FT_SSL sha256 -q)" \
        "$(echo -n "" | openssl sha256 | awk '{print $2}')"

    check "-q fichier" \
        "$($FT_SSL sha256 -q /tmp/ft_ssl_test_file)" \
        "$(openssl sha256 /tmp/ft_ssl_test_file | awk '{print $2}')"

    check "-q fichier 64 chars" \
        "$($FT_SSL sha256 -q /tmp/ft_ssl_test_64a)" \
        "$(openssl sha256 /tmp/ft_ssl_test_64a | awk '{print $2}')"
}

# ================================================================
# SHA-256 FLAG -r
# ================================================================

test_sha256_flag_r()
{
    echo ""
    echo "=== SHA256 FLAG -r ==="

    check "-r fichier hash" \
        "$($FT_SSL sha256 -r /tmp/ft_ssl_test_file | awk '{print $1}')" \
        "$(openssl sha256 /tmp/ft_ssl_test_file | awk '{print $2}')"

    check "-r -s foo hash" \
        "$($FT_SSL sha256 -r -s "foo" | awk '{print $1}')" \
        "$(echo -n "foo" | openssl sha256 | awk '{print $2}')"

    check "-r stdin hash" \
        "$(echo -n "abc" | $FT_SSL sha256 -r | awk '{print $1}')" \
        "$(echo -n "abc" | openssl sha256 | awk '{print $2}')"
}

# ================================================================
# SHA-256 FICHIERS — comparaison directe openssl vs ft_ssl
# ================================================================

test_sha256_files()
{
    echo ""
    echo "=== SHA256 FICHIERS ==="

    check "fichier test_file" \
        "$($FT_SSL sha256 /tmp/ft_ssl_test_file)" \
        "$(openssl sha256 /tmp/ft_ssl_test_file)"

    check "fichier test_website" \
        "$($FT_SSL sha256 /tmp/ft_ssl_test_website)" \
        "$(openssl sha256 /tmp/ft_ssl_test_website)"

    check "fichier 57 chars" \
        "$($FT_SSL sha256 /tmp/ft_ssl_test_57a)" \
        "$(openssl sha256 /tmp/ft_ssl_test_57a)"

    check "fichier 64 chars" \
        "$($FT_SSL sha256 /tmp/ft_ssl_test_64a)" \
        "$(openssl sha256 /tmp/ft_ssl_test_64a)"

    check_manual "fichier inexistant" \
        "ft_ssl: sha256: /tmp/inexistant: No such file or directory" \
        "$($FT_SSL sha256 /tmp/inexistant 2>&1)"
}

# ================================================================
# SHA-256 FLAG -p
# ================================================================

test_sha256_flag_p()
{
    echo ""
    echo "=== SHA256 FLAG -p ==="

    check "-p abc hash correct" \
        "$(echo -n "abc" | $FT_SSL sha256 -p | awk -F'= ' '{print $2}')" \
        "$(echo -n "abc" | openssl sha256 | awk '{print $2}')"

    check "-p avec newline hash correct" \
        "$(echo "42 is nice" | $FT_SSL sha256 -p | awk -F'= ' '{print $2}')" \
        "$(echo "42 is nice" | openssl sha256 | awk '{print $2}')"

    check "-p + fichier hash stdin correct" \
        "$(echo "be sure" | $FT_SSL sha256 -p /tmp/ft_ssl_test_file | head -1 | awk -F'= ' '{print $2}')" \
        "$(echo "be sure" | openssl sha256 | awk '{print $2}')"

    check "-p + fichier hash fichier correct" \
        "$(echo "be sure" | $FT_SSL sha256 -p /tmp/ft_ssl_test_file | tail -1 | awk -F'= ' '{print $2}')" \
        "$(openssl sha256 /tmp/ft_ssl_test_file | awk '{print $2}')"
}

# ================================================================
# SHA-256 COMBINAISONS
# ================================================================

test_sha256_combinations()
{
    echo ""
    echo "=== SHA256 COMBINAISONS ==="

    check "-p -r stdin hash" \
        "$(echo "but eventually" | $FT_SSL sha256 -p -r /tmp/ft_ssl_test_file | head -1 | awk -F'= ' '{print $2}')" \
        "$(echo "but eventually" | openssl sha256 | awk '{print $2}')"

    check "-p -r fichier hash" \
        "$(echo "but eventually" | $FT_SSL sha256 -p -r /tmp/ft_ssl_test_file | tail -1 | awk '{print $1}')" \
        "$(openssl sha256 /tmp/ft_ssl_test_file | awk '{print $2}')"

    check "-p -s foo fichier hash stdin" \
        "$(echo "GL HF" | $FT_SSL sha256 -p -s "foo" /tmp/ft_ssl_test_file | head -1 | awk -F'= ' '{print $2}')" \
        "$(echo "GL HF" | openssl sha256 | awk '{print $2}')"

    check "-p -s foo hash -s" \
        "$(echo "GL HF" | $FT_SSL sha256 -p -s "foo" /tmp/ft_ssl_test_file | sed -n '2p' | awk -F'= ' '{print $2}')" \
        "$(echo -n "foo" | openssl sha256 | awk '{print $2}')"

    check "-q -p content affiché" \
        "$(echo -n "just to be extra clear" | $FT_SSL sha256 -q -p | head -1)" \
        "just to be extra clear"

    check "-q -p hash correct" \
        "$(echo -n "just to be extra clear" | $FT_SSL sha256 -q -p | tail -1)" \
        "$(echo -n "just to be extra clear" | openssl sha256 | awk '{print $2}')"
}

# ================================================================
# MD5 — régression complète
# ================================================================

test_md5_regression()
{
    echo ""
    echo "=== MD5 REGRESSION ==="

    check "stdin abc" \
        "$(echo -n "abc" | $FT_SSL md5)" \
        "$(echo -n "abc" | openssl md5)"

    check "stdin vide" \
        "$(echo -n "" | $FT_SSL md5)" \
        "$(echo -n "" | openssl md5)"

    check "stdin long" \
        "$(echo -n "The quick brown fox jumps over the lazy dog" | $FT_SSL md5)" \
        "$(echo -n "The quick brown fox jumps over the lazy dog" | openssl md5)"

    check "fichier test_file" \
        "$($FT_SSL md5 /tmp/ft_ssl_test_file)" \
        "$(openssl md5 /tmp/ft_ssl_test_file)"

    check "-q abc" \
        "$(echo -n "abc" | $FT_SSL md5 -q)" \
        "$(echo -n "abc" | openssl md5 | awk '{print $2}')"

    check "-q fichier" \
        "$($FT_SSL md5 -q /tmp/ft_ssl_test_file)" \
        "$(openssl md5 /tmp/ft_ssl_test_file | awk '{print $2}')"

    check_manual "-s abc" \
        'MD5("abc")= 900150983cd24fb0d6963f7d28e17f72' \
        "$($FT_SSL md5 -s "abc")"

    check_manual "-r fichier" \
        "53d53ea94217b259c11a5a2d104ec58a /tmp/ft_ssl_test_file" \
        "$($FT_SSL md5 -r /tmp/ft_ssl_test_file)"
}

# ================================================================
# ERREURS
# ================================================================

test_errors()
{
    echo ""
    echo "=== ERREURS ==="

    check_manual "no args" \
        "usage: ft_ssl command [flags] [file/string]" \
        "$($FT_SSL 2>&1)"

    check_manual "commande invalide" \
        "ft_ssl: Error: 'foobar' is an invalid command.

Commands:
md5
sha256

Flags:
-p -q -r -s" \
        "$($FT_SSL foobar 2>&1)"
}

# ================================================================
# résumé
# ================================================================

summary()
{
    echo ""
    echo "================================"
    echo "TOTAL  : $TOTAL"
    echo -e "PASS   : ${GREEN}$PASS${RESET}"
    if [ $FAIL -gt 0 ]; then
        echo -e "FAIL   : ${RED}$FAIL${RESET}"
    else
        echo "FAIL   : $FAIL"
    fi
    echo "================================"
}

# ================================================================
# main
# ================================================================

setup
test_sha256_stdin
test_sha256_flag_s
test_sha256_flag_q
test_sha256_flag_r
test_sha256_files
test_sha256_flag_p
test_sha256_combinations
test_md5_regression
test_errors
summary
cleanup