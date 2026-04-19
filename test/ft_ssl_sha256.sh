#!/bin/bash

# ================================================================
# test_ft_ssl_md5.sh — Tests MD5 avec nouveau format -p
# Usage : ./test_ft_ssl_md5.sh [chemin vers ft_ssl]
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
    echo "And above all," > /tmp/ft_ssl_test_file
    echo "https://www.42.fr/" > /tmp/ft_ssl_test_website
}

cleanup()
{
    rm -f /tmp/ft_ssl_test_file
    rm -f /tmp/ft_ssl_test_website
}

test_stdin()
{
    echo "=== STDIN ==="

    check "stdin abc (sans newline)" \
        "$(echo -n "abc" | $FT_SSL md5)" \
        "$(echo -n "abc" | openssl md5)"

    check "stdin 42 is nice (avec newline)" \
        "$(echo "42 is nice" | $FT_SSL md5)" \
        "$(echo "42 is nice" | openssl md5)"

    check "stdin vide" \
        "$(echo -n "" | $FT_SSL md5)" \
        "$(echo -n "" | openssl md5)"

    check "stdin long message" \
        "$(echo -n "The quick brown fox jumps over the lazy dog" | $FT_SSL md5)" \
        "$(echo -n "The quick brown fox jumps over the lazy dog" | openssl md5)"

    check "stdin 55 chars" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL md5)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl md5)"

    check "stdin 64 chars" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL md5)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl md5)"

    check "stdin 65 chars" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL md5)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl md5)"
}

test_flag_s()
{
    echo ""
    echo "=== FLAG -s ==="

    check_manual "-s abc" \
        'MD5("abc")= 900150983cd24fb0d6963f7d28e17f72' \
        "$($FT_SSL md5 -s "abc")"

    check_manual "-s foo" \
        'MD5("foo")= acbd18db4cc2f85cedef654fccc4a4d8' \
        "$($FT_SSL md5 -s "foo")"

    check_manual "-s vide" \
        'MD5("")= d41d8cd98f00b204e9800998ecf8427e' \
        "$($FT_SSL md5 -s "")"

    check_manual "-s pity..." \
        'MD5("pity those that aren'"'"'t following baerista on spotify.")= a3c990a1964705d9bf0e602f44572f5f' \
        "$($FT_SSL md5 -s "pity those that aren't following baerista on spotify.")"

    check "-s abc hash via openssl" \
        "$(echo -n "abc" | $FT_SSL md5 -q)" \
        "$(echo -n "abc" | openssl md5 | awk '{print $2}')"
}

test_flag_q()
{
    echo ""
    echo "=== FLAG -q ==="

    check "-q stdin abc" \
        "$(echo -n "abc" | $FT_SSL md5 -q)" \
        "$(echo -n "abc" | openssl md5 | awk '{print $2}')"

    check "-q stdin vide" \
        "$(echo -n "" | $FT_SSL md5 -q)" \
        "$(echo -n "" | openssl md5 | awk '{print $2}')"

    check "-q fichier" \
        "$($FT_SSL md5 -q /tmp/ft_ssl_test_file)" \
        "$(openssl md5 /tmp/ft_ssl_test_file | awk '{print $2}')"

    check_manual "-q -r stdin" \
        "e20c3b973f63482a778f3fd1869b7f25" \
        "$(echo "Pity the living." | $FT_SSL md5 -q -r)"
}

test_flag_r()
{
    echo ""
    echo "=== FLAG -r ==="

    check_manual "-r fichier" \
        "53d53ea94217b259c11a5a2d104ec58a /tmp/ft_ssl_test_file" \
        "$($FT_SSL md5 -r /tmp/ft_ssl_test_file)"

    check_manual "-r -s foo" \
        'acbd18db4cc2f85cedef654fccc4a4d8 "foo"' \
        "$($FT_SSL md5 -r -s "foo")"

    check "-r fichier hash correct" \
        "$($FT_SSL md5 -r /tmp/ft_ssl_test_file | awk '{print $1}')" \
        "$(openssl md5 /tmp/ft_ssl_test_file | awk '{print $2}')"
}

test_files()
{
    echo ""
    echo "=== FICHIERS ==="

    check "fichier test_file" \
        "$($FT_SSL md5 /tmp/ft_ssl_test_file)" \
        "$(openssl md5 /tmp/ft_ssl_test_file)"

    check "fichier test_website" \
        "$($FT_SSL md5 /tmp/ft_ssl_test_website)" \
        "$(openssl md5 /tmp/ft_ssl_test_website)"

    check_manual "fichier inexistant" \
        "ft_ssl: md5: /tmp/inexistant: No such file or directory" \
        "$($FT_SSL md5 /tmp/inexistant 2>&1)"
}

test_flag_p()
{
    echo ""
    echo "=== FLAG -p ==="

    # Test que -p affiche bien 2 lignes
    check "-p abc nb lignes" \
        "$(echo -n "abc" | $FT_SSL md5 -p | wc -l)" \
        "2"

    # Test que la première ligne est bien le contenu
    check "-p abc ligne 1" \
        "$(echo -n "abc" | $FT_SSL md5 -p | sed -n '1p')" \
        "abc"

    # Test que la deuxième ligne contient le bon hash
    check "-p abc hash" \
        "$(echo -n "abc" | $FT_SSL md5 -p | sed -n '2p')" \
        "MD5(stdin)= $(echo -n "abc" | openssl md5 | awk '{print $2}')"

    # Test avec newline
    check "-p avec newline nb lignes" \
        "$(echo "42 is nice" | $FT_SSL md5 -p | wc -l)" \
        "2"

    check "-p avec newline ligne 1" \
        "$(echo "42 is nice" | $FT_SSL md5 -p | sed -n '1p')" \
        "42 is nice"

    check "-p avec newline hash" \
        "$(echo "42 is nice" | $FT_SSL md5 -p | sed -n '2p')" \
        "MD5(stdin)= $(echo "42 is nice" | openssl md5 | awk '{print $2}')"

    # Test -p avec fichier
    check "-p + fichier nb lignes" \
        "$(echo "be sure" | $FT_SSL md5 -p /tmp/ft_ssl_test_file | wc -l)" \
        "3"

    check "-p + fichier hash stdin" \
        "$(echo "be sure" | $FT_SSL md5 -p /tmp/ft_ssl_test_file | sed -n '2p')" \
        "MD5(stdin)= $(echo "be sure" | openssl md5 | awk '{print $2}')"

    check "-p + fichier hash fichier" \
        "$(echo "be sure" | $FT_SSL md5 -p /tmp/ft_ssl_test_file | tail -1)" \
        "$(openssl md5 /tmp/ft_ssl_test_file)"
}

test_combinations()
{
    echo ""
    echo "=== COMBINAISONS ==="

    check "-p -r nb lignes" \
        "$(echo "but eventually" | $FT_SSL md5 -p -r /tmp/ft_ssl_test_file | wc -l)" \
        "3"

    check "-p -r stdin hash" \
        "$(echo "but eventually" | $FT_SSL md5 -p -r /tmp/ft_ssl_test_file | sed -n '2p')" \
        "$(echo "but eventually" | openssl md5 | awk '{print $2}') (stdin)"

    check "-p -r fichier hash" \
        "$(echo "but eventually" | $FT_SSL md5 -p -r /tmp/ft_ssl_test_file | tail -1)" \
        "$(openssl md5 /tmp/ft_ssl_test_file | awk '{print $2}') /tmp/ft_ssl_test_file"

    check "-p -s foo nb lignes" \
        "$(echo "GL HF" | $FT_SSL md5 -p -s "foo" /tmp/ft_ssl_test_file | wc -l)" \
        "4"

    check "-p -s foo stdin hash" \
        "$(echo "GL HF" | $FT_SSL md5 -p -s "foo" /tmp/ft_ssl_test_file | sed -n '2p')" \
        "MD5(stdin)= $(echo "GL HF" | openssl md5 | awk '{print $2}')"

    check "-p -s foo string hash" \
        "$(echo "GL HF" | $FT_SSL md5 -p -s "foo" /tmp/ft_ssl_test_file | sed -n '3p')" \
        'MD5("foo")= acbd18db4cc2f85cedef654fccc4a4d8'

    check "-q -p nb lignes" \
        "$(echo -n "clear" | $FT_SSL md5 -q -p | wc -l)" \
        "2"

    check "-q -p ligne 1" \
        "$(echo -n "clear" | $FT_SSL md5 -q -p | sed -n '1p')" \
        "clear"

    check "-q -p hash" \
        "$(echo -n "clear" | $FT_SSL md5 -q -p | tail -1)" \
        "$(echo -n "clear" | openssl md5 | awk '{print $2}')"

    check "-r -q stdin" \
        "$(echo -n "abc" | $FT_SSL md5 -r -q)" \
        "$(echo -n "abc" | openssl md5 | awk '{print $2}')"
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
test_stdin
test_flag_s
test_flag_q
test_flag_r
test_files
test_flag_p
test_combinations
test_errors
summary
cleanup

exit $FAIL