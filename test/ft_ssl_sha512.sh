#!/bin/bash

# ================================================================
# test_ft_ssl_sha512.sh — Comparaison ft_ssl sha512 vs openssl
# Usage : ./test_ft_ssl_sha512.sh [chemin vers ft_ssl]
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
    echo "And above all,"           > /tmp/ft_ssl_test_file
    echo "https://www.42.fr/"       > /tmp/ft_ssl_test_website
    printf 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' \
        > /tmp/ft_ssl_test_111a
    printf 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' \
        > /tmp/ft_ssl_test_128a
    printf 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' \
        > /tmp/ft_ssl_test_129a
}

cleanup()
{
    rm -f /tmp/ft_ssl_test_file
    rm -f /tmp/ft_ssl_test_website
    rm -f /tmp/ft_ssl_test_111a
    rm -f /tmp/ft_ssl_test_128a
    rm -f /tmp/ft_ssl_test_129a
}

# ================================================================
# SHA-512 STDIN — comparaison directe openssl vs ft_ssl
# ================================================================

test_sha512_stdin()
{
    echo "=== SHA512 STDIN ==="

    check "stdin abc" \
        "$(echo -n "abc" | $FT_SSL sha512)" \
        "$(echo -n "abc" | openssl sha512)"

    check "stdin vide" \
        "$(echo -n "" | $FT_SSL sha512)" \
        "$(echo -n "" | openssl sha512)"

    check "stdin avec newline" \
        "$(echo "42 is nice" | $FT_SSL sha512)" \
        "$(echo "42 is nice" | openssl sha512)"

    check "stdin 111 chars (< 112)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL sha512)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl sha512)"

    check "stdin 112 chars (limite)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL sha512)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl sha512)"

    check "stdin 128 chars (bloc plein)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL sha512)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl sha512)"

    check "stdin 129 chars (bloc + 1)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL sha512)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl sha512)"

    check "stdin long message" \
        "$(echo -n "The quick brown fox jumps over the lazy dog" | $FT_SSL sha512)" \
        "$(echo -n "The quick brown fox jumps over the lazy dog" | openssl sha512)"

    check "stdin pity..." \
        "$(echo -n "pity those that aren't following baerista on spotify." | $FT_SSL sha512)" \
        "$(echo -n "pity those that aren't following baerista on spotify." | openssl sha512)"
}

# ================================================================
# SHA-512 FLAG -s
# ================================================================

test_sha512_flag_s()
{
    echo ""
    echo "=== SHA512 FLAG -s ==="

    check "-s abc hash" \
        "$(echo -n "abc" | $FT_SSL sha512 -q)" \
        "$(echo -n "abc" | openssl sha512 | awk '{print $2}')"

    check "-s foo hash" \
        "$(echo -n "foo" | $FT_SSL sha512 -q)" \
        "$(echo -n "foo" | openssl sha512 | awk '{print $2}')"

    check "-s vide hash" \
        "$(echo -n "" | $FT_SSL sha512 -q)" \
        "$(echo -n "" | openssl sha512 | awk '{print $2}')"

    check "-s pity... hash" \
        "$(echo -n "pity those that aren't following baerista on spotify." | $FT_SSL sha512 -q)" \
        "$(echo -n "pity those that aren't following baerista on spotify." | openssl sha512 | awk '{print $2}')"
}

# ================================================================
# SHA-512 FLAG -q
# ================================================================

test_sha512_flag_q()
{
    echo ""
    echo "=== SHA512 FLAG -q ==="

    check "-q stdin abc" \
        "$(echo -n "abc" | $FT_SSL sha512 -q)" \
        "$(echo -n "abc" | openssl sha512 | awk '{print $2}')"

    check "-q stdin vide" \
        "$(echo -n "" | $FT_SSL sha512 -q)" \
        "$(echo -n "" | openssl sha512 | awk '{print $2}')"

    check "-q fichier" \
        "$($FT_SSL sha512 -q /tmp/ft_ssl_test_file)" \
        "$(openssl sha512 /tmp/ft_ssl_test_file | awk '{print $2}')"

    check "-q fichier 128 chars" \
        "$($FT_SSL sha512 -q /tmp/ft_ssl_test_128a)" \
        "$(openssl sha512 /tmp/ft_ssl_test_128a | awk '{print $2}')"
}

# ================================================================
# SHA-512 FLAG -r
# ================================================================

test_sha512_flag_r()
{
    echo ""
    echo "=== SHA512 FLAG -r ==="

    check "-r fichier hash" \
        "$($FT_SSL sha512 -r /tmp/ft_ssl_test_file | awk '{print $1}')" \
        "$(openssl sha512 /tmp/ft_ssl_test_file | awk '{print $2}')"

    check "-r -s foo hash" \
        "$($FT_SSL sha512 -r -s "foo" | awk '{print $1}')" \
        "$(echo -n "foo" | openssl sha512 | awk '{print $2}')"

    check "-r stdin hash" \
        "$(echo -n "abc" | $FT_SSL sha512 -r | awk '{print $1}')" \
        "$(echo -n "abc" | openssl sha512 | awk '{print $2}')"
}

# ================================================================
# SHA-512 FICHIERS — comparaison directe openssl vs ft_ssl
# ================================================================

test_sha512_files()
{
    echo ""
    echo "=== SHA512 FICHIERS ==="

    check "fichier test_file" \
        "$($FT_SSL sha512 /tmp/ft_ssl_test_file)" \
        "$(openssl sha512 /tmp/ft_ssl_test_file)"

    check "fichier test_website" \
        "$($FT_SSL sha512 /tmp/ft_ssl_test_website)" \
        "$(openssl sha512 /tmp/ft_ssl_test_website)"

    check "fichier 111 chars" \
        "$($FT_SSL sha512 /tmp/ft_ssl_test_111a)" \
        "$(openssl sha512 /tmp/ft_ssl_test_111a)"

    check "fichier 128 chars" \
        "$($FT_SSL sha512 /tmp/ft_ssl_test_128a)" \
        "$(openssl sha512 /tmp/ft_ssl_test_128a)"

    check "fichier 129 chars" \
        "$($FT_SSL sha512 /tmp/ft_ssl_test_129a)" \
        "$(openssl sha512 /tmp/ft_ssl_test_129a)"

    check_manual "fichier inexistant" \
        "ft_ssl: sha512: /tmp/inexistant: No such file or directory" \
        "$($FT_SSL sha512 /tmp/inexistant 2>&1)"
}

# ================================================================
# SHA-512 FLAG -p
# ================================================================

test_sha512_flag_p()
{
    echo ""
    echo "=== SHA512 FLAG -p ==="

    check "-p abc hash correct" \
        "$(echo -n "abc" | $FT_SSL sha512 -p | awk -F'= ' '{print $2}')" \
        "$(echo -n "abc" | openssl sha512 | awk '{print $2}')"

    check "-p avec newline hash correct" \
        "$(echo "42 is nice" | $FT_SSL sha512 -p | awk -F'= ' '{print $2}')" \
        "$(echo "42 is nice" | openssl sha512 | awk '{print $2}')"

    check "-p + fichier hash stdin" \
        "$(echo "be sure" | $FT_SSL sha512 -p /tmp/ft_ssl_test_file | head -1 | awk -F'= ' '{print $2}')" \
        "$(echo "be sure" | openssl sha512 | awk '{print $2}')"

    check "-p + fichier hash fichier" \
        "$(echo "be sure" | $FT_SSL sha512 -p /tmp/ft_ssl_test_file | tail -1 | awk -F'= ' '{print $2}')" \
        "$(openssl sha512 /tmp/ft_ssl_test_file | awk '{print $2}')"
}

# ================================================================
# SHA-512 COMBINAISONS
# ================================================================

test_sha512_combinations()
{
    echo ""
    echo "=== SHA512 COMBINAISONS ==="

    check "-p -r stdin hash" \
        "$(echo "but eventually" | $FT_SSL sha512 -p -r /tmp/ft_ssl_test_file | head -1 | awk -F'= ' '{print $2}')" \
        "$(echo "but eventually" | openssl sha512 | awk '{print $2}')"

    check "-p -r fichier hash" \
        "$(echo "but eventually" | $FT_SSL sha512 -p -r /tmp/ft_ssl_test_file | tail -1 | awk '{print $1}')" \
        "$(openssl sha512 /tmp/ft_ssl_test_file | awk '{print $2}')"

    check "-p -s foo hash stdin" \
        "$(echo "GL HF" | $FT_SSL sha512 -p -s "foo" /tmp/ft_ssl_test_file | head -1 | awk -F'= ' '{print $2}')" \
        "$(echo "GL HF" | openssl sha512 | awk '{print $2}')"

    check "-p -s foo hash -s" \
        "$(echo "GL HF" | $FT_SSL sha512 -p -s "foo" /tmp/ft_ssl_test_file | sed -n '2p' | awk -F'= ' '{print $2}')" \
        "$(echo -n "foo" | openssl sha512 | awk '{print $2}')"

    check "-q -p content affiché" \
        "$(echo -n "just to be extra clear" | $FT_SSL sha512 -q -p | head -1)" \
        "just to be extra clear"

    check "-q -p hash correct" \
        "$(echo -n "just to be extra clear" | $FT_SSL sha512 -q -p | tail -1)" \
        "$(echo -n "just to be extra clear" | openssl sha512 | awk '{print $2}')"
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
test_sha512_stdin
test_sha512_flag_s
test_sha512_flag_q
test_sha512_flag_r
test_sha512_files
test_sha512_flag_p
test_sha512_combinations
summary
cleanup