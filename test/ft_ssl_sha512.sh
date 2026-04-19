#!/bin/bash

# ================================================================
# test_ft_ssl_sha512.sh — Tests SHA512 avec nouveau format -p
# Usage : ./test_ft_ssl_sha512.sh [chemin vers ft_ssl]
# ================================================================

FT_SSL=${1:-"../ft_ssl"}
PASS=0
FAIL=0
TOTAL=0

GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'

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

test_sha512_flag_s()
{
    echo ""
    echo "=== SHA512 FLAG -s ==="

    check "-s abc" \
        "$($FT_SSL sha512 -s "abc")" \
        "SHA2-512(\"abc\")= $(echo -n "abc" | openssl sha512 | awk '{print $2}')"

    check "-s foo" \
        "$($FT_SSL sha512 -s "foo")" \
        "SHA2-512(\"foo\")= $(echo -n "foo" | openssl sha512 | awk '{print $2}')"

    check "-s vide" \
        "$($FT_SSL sha512 -s "")" \
        "SHA2-512(\"\")= $(echo -n "" | openssl sha512 | awk '{print $2}')"

    check "-s pity..." \
        "$($FT_SSL sha512 -s "pity those that aren't following baerista on spotify.")" \
        "SHA2-512(\"pity those that aren't following baerista on spotify.\")= $(echo -n "pity those that aren't following baerista on spotify." | openssl sha512 | awk '{print $2}')"
}

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

    check "-q -s abc" \
        "$($FT_SSL sha512 -q -s "abc")" \
        "$(echo -n "abc" | openssl sha512 | awk '{print $2}')"
}

test_sha512_flag_r()
{
    echo ""
    echo "=== SHA512 FLAG -r ==="

    check "-r fichier" \
        "$($FT_SSL sha512 -r /tmp/ft_ssl_test_file)" \
        "$(openssl sha512 /tmp/ft_ssl_test_file | awk '{print $2}') /tmp/ft_ssl_test_file"

    check "-r -s foo" \
        "$($FT_SSL sha512 -r -s "foo")" \
        "$(echo -n "foo" | openssl sha512 | awk '{print $2}') \"foo\""

    check "-r stdin" \
        "$(echo -n "abc" | $FT_SSL sha512 -r)" \
        "$(echo -n "abc" | openssl sha512 | awk '{print $2}') (stdin)"
}

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

    check "plusieurs fichiers nb lignes" \
        "$($FT_SSL sha512 /tmp/ft_ssl_test_file /tmp/ft_ssl_test_website | wc -l)" \
        "$(openssl sha512 /tmp/ft_ssl_test_file /tmp/ft_ssl_test_website | wc -l)"
}

test_sha512_flag_p()
{
    echo ""
    echo "=== SHA512 FLAG -p ==="

    # Test que -p affiche bien 2 lignes
    check "-p abc nb lignes" \
        "$(echo -n "abc" | $FT_SSL sha512 -p | wc -l)" \
        "2"

    # Test que la première ligne est bien le contenu
    check "-p abc ligne 1" \
        "$(echo -n "abc" | $FT_SSL sha512 -p | sed -n '1p')" \
        "abc"

    # Test que la deuxième ligne contient le bon hash
    check "-p abc hash" \
        "$(echo -n "abc" | $FT_SSL sha512 -p | sed -n '2p')" \
        "SHA2-512(stdin)= $(echo -n "abc" | openssl sha512 | awk '{print $2}')"

    # Test avec newline
    check "-p avec newline nb lignes" \
        "$(echo "42 is nice" | $FT_SSL sha512 -p | wc -l)" \
        "2"

    check "-p avec newline ligne 1" \
        "$(echo "42 is nice" | $FT_SSL sha512 -p | sed -n '1p')" \
        "42 is nice"

    check "-p avec newline hash" \
        "$(echo "42 is nice" | $FT_SSL sha512 -p | sed -n '2p')" \
        "SHA2-512(stdin)= $(echo "42 is nice" | openssl sha512 | awk '{print $2}')"

    # Test -p avec fichier
    check "-p + fichier nb lignes" \
        "$(echo "be sure" | $FT_SSL sha512 -p /tmp/ft_ssl_test_file | wc -l)" \
        "3"

    check "-p + fichier hash stdin" \
        "$(echo "be sure" | $FT_SSL sha512 -p /tmp/ft_ssl_test_file | sed -n '2p')" \
        "SHA2-512(stdin)= $(echo "be sure" | openssl sha512 | awk '{print $2}')"

    check "-p + fichier hash fichier" \
        "$(echo "be sure" | $FT_SSL sha512 -p /tmp/ft_ssl_test_file | tail -1)" \
        "$(openssl sha512 /tmp/ft_ssl_test_file)"
}

test_sha512_combinations()
{
    echo ""
    echo "=== SHA512 COMBINAISONS ==="

    check "-p -r nb lignes" \
        "$(echo "but eventually" | $FT_SSL sha512 -p -r /tmp/ft_ssl_test_file | wc -l)" \
        "3"

    check "-p -r stdin hash" \
        "$(echo "but eventually" | $FT_SSL sha512 -p -r /tmp/ft_ssl_test_file | sed -n '2p')" \
        "$(echo "but eventually" | openssl sha512 | awk '{print $2}') (stdin)"

    check "-p -r fichier hash" \
        "$(echo "but eventually" | $FT_SSL sha512 -p -r /tmp/ft_ssl_test_file | tail -1)" \
        "$(openssl sha512 /tmp/ft_ssl_test_file | awk '{print $2}') /tmp/ft_ssl_test_file"

    check "-p -s foo nb lignes" \
        "$(echo "GL HF" | $FT_SSL sha512 -p -s "foo" /tmp/ft_ssl_test_file | wc -l)" \
        "4"

    check "-p -s foo stdin hash" \
        "$(echo "GL HF" | $FT_SSL sha512 -p -s "foo" /tmp/ft_ssl_test_file | sed -n '2p')" \
        "SHA2-512(stdin)= $(echo "GL HF" | openssl sha512 | awk '{print $2}')"

    check "-p -s foo string hash" \
        "$(echo "GL HF" | $FT_SSL sha512 -p -s "foo" /tmp/ft_ssl_test_file | sed -n '3p')" \
        "SHA2-512(\"foo\")= $(echo -n "foo" | openssl sha512 | awk '{print $2}')"

    check "-q -p nb lignes" \
        "$(echo -n "clear" | $FT_SSL sha512 -q -p | wc -l)" \
        "2"

    check "-q -p ligne 1" \
        "$(echo -n "clear" | $FT_SSL sha512 -q -p | sed -n '1p')" \
        "clear"

    check "-q -p hash" \
        "$(echo -n "clear" | $FT_SSL sha512 -q -p | tail -1)" \
        "$(echo -n "clear" | openssl sha512 | awk '{print $2}')"

    check "-r -q stdin" \
        "$(echo -n "abc" | $FT_SSL sha512 -r -q)" \
        "$(echo -n "abc" | openssl sha512 | awk '{print $2}')"

    check "-r -q -s foo" \
        "$($FT_SSL sha512 -r -q -s "foo")" \
        "$(echo -n "foo" | openssl sha512 | awk '{print $2}')"
}

test_sha512_edge_cases()
{
    echo ""
    echo "=== SHA512 EDGE CASES ==="

    check "message 1 byte" \
        "$(echo -n "a" | $FT_SSL sha512 -q)" \
        "$(echo -n "a" | openssl sha512 | awk '{print $2}')"

    check "message 2 bytes" \
        "$(echo -n "ab" | $FT_SSL sha512 -q)" \
        "$(echo -n "ab" | openssl sha512 | awk '{print $2}')"

    check "message 111 bytes" \
        "$(python3 -c "print('a'*111, end='')" | $FT_SSL sha512 -q)" \
        "$(python3 -c "print('a'*111, end='')" | openssl sha512 | awk '{print $2}')"

    check "message 112 bytes" \
        "$(python3 -c "print('a'*112, end='')" | $FT_SSL sha512 -q)" \
        "$(python3 -c "print('a'*112, end='')" | openssl sha512 | awk '{print $2}')"

    check "message 113 bytes" \
        "$(python3 -c "print('a'*113, end='')" | $FT_SSL sha512 -q)" \
        "$(python3 -c "print('a'*113, end='')" | openssl sha512 | awk '{print $2}')"

    check "caractères spéciaux" \
        "$(echo -n "!@#$%^&*()_+-=[]{}|;:,.<>?" | $FT_SSL sha512 -q)" \
        "$(echo -n "!@#$%^&*()_+-=[]{}|;:,.<>?" | openssl sha512 | awk '{print $2}')"
}

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
sha512
whirlpool

Flags:
-p -q -r -s" \
        "$($FT_SSL foobar 2>&1)"
}

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
    
    if [ $FAIL -eq 0 ]; then
        echo -e "${GREEN}✓ Tous les tests passent !${RESET}"
    else
        echo -e "${RED}✗ $FAIL test(s) échoué(s)${RESET}"
    fi
}

setup
test_sha512_stdin
test_sha512_flag_s
test_sha512_flag_q
test_sha512_flag_r
test_sha512_files
test_sha512_flag_p
test_sha512_combinations
test_sha512_edge_cases
test_errors
summary
cleanup

exit $FAIL