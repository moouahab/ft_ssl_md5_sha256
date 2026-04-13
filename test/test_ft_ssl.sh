#!/bin/bash

# ================================================================
# test_ft_ssl.sh — Comparaison ft_ssl vs openssl
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

# compare ft_ssl vs openssl directement
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

# compare ft_ssl vs valeur attendue fixe
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

# ================================================================
# STDIN — comparaison directe openssl vs ft_ssl
# ================================================================

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

# ================================================================
# FLAG -s — openssl -s ne marche pas sur certaines versions
#           → valeurs vérifiées manuellement contre openssl
# ================================================================

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

    # vérifier le hash seul via openssl stdin
    check "-s abc hash via openssl" \
        "$(echo -n "abc" | $FT_SSL md5 -q)" \
        "$(echo -n "abc" | openssl md5 | awk '{print $2}')"
}

# ================================================================
# FLAG -q
# ================================================================

test_flag_q()
{
    echo ""
    echo "=== FLAG -q ==="

    # hash seul = même valeur que openssl
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

# ================================================================
# FLAG -r
# ================================================================

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

    # hash -r = même valeur que openssl
    check "-r fichier hash correct" \
        "$($FT_SSL md5 -r /tmp/ft_ssl_test_file | awk '{print $1}')" \
        "$(openssl md5 /tmp/ft_ssl_test_file | awk '{print $2}')"
}

# ================================================================
# FICHIERS — comparaison directe openssl vs ft_ssl
# ================================================================

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

# ================================================================
# FLAG -p — pas d'équivalent openssl
#           → hash vérifié via openssl stdin séparément
# ================================================================

test_flag_p()
{
    echo ""
    echo "=== FLAG -p ==="

    # hash dans -p = même valeur que openssl stdin
    check "-p abc hash correct" \
        "$(echo -n "abc" | $FT_SSL md5 -p | awk -F'= ' '{print $2}')" \
        "$(echo -n "abc" | openssl md5 | awk '{print $2}')"

    check "-p avec newline hash correct" \
        "$(echo "42 is nice" | $FT_SSL md5 -p | awk -F'= ' '{print $2}')" \
        "$(echo "42 is nice" | openssl md5 | awk '{print $2}')"

    # format label
    check_manual "-p abc format label" \
        '("abc")= 900150983cd24fb0d6963f7d28e17f72' \
        "$(echo -n "abc" | $FT_SSL md5 -p)"

    check_manual "-p + fichier" \
        '("be sure to handle edge cases carefully")= 3553dc7dc5963b583c056d1b9fa3349c
MD5(/tmp/ft_ssl_test_file)= 53d53ea94217b259c11a5a2d104ec58a' \
        "$(echo "be sure to handle edge cases carefully" | $FT_SSL md5 -p /tmp/ft_ssl_test_file)"
}

# ================================================================
# COMBINAISONS — hash vérifié via openssl
# ================================================================

test_combinations()
{
    echo ""
    echo "=== COMBINAISONS ==="

    # vérifier hash stdin dans combinaisons = openssl
    check "-p -r hash stdin correct" \
        "$(echo "but eventually you will understand" | $FT_SSL md5 -p -r /tmp/ft_ssl_test_file | head -1 | awk -F'= ' '{print $2}')" \
        "$(echo "but eventually you will understand" | openssl md5 | awk '{print $2}')"

    check_manual "-p -r fichier format" \
        '("but eventually you will understand")= dcdd84e0f635694d2a943fa8d3905281
53d53ea94217b259c11a5a2d104ec58a /tmp/ft_ssl_test_file' \
        "$(echo "but eventually you will understand" | $FT_SSL md5 -p -r /tmp/ft_ssl_test_file)"

    check_manual "-p -s foo fichier" \
        '("GL HF let'"'"'s go")= d1e3cc342b6da09480b27ec57ff243e2
MD5("foo")= acbd18db4cc2f85cedef654fccc4a4d8
MD5(/tmp/ft_ssl_test_file)= 53d53ea94217b259c11a5a2d104ec58a' \
        "$(echo "GL HF let's go" | $FT_SSL md5 -p -s "foo" /tmp/ft_ssl_test_file)"

    check_manual "-r -p -s foo fichier -s bar" \
        '("one more thing")= a0bd1876c6f011dd50fae52827f445f5
acbd18db4cc2f85cedef654fccc4a4d8 "foo"
53d53ea94217b259c11a5a2d104ec58a /tmp/ft_ssl_test_file
ft_ssl: md5: -s: No such file or directory
ft_ssl: md5: bar: No such file or directory' \
        "$(echo "one more thing" | $FT_SSL md5 -r -p -s "foo" /tmp/ft_ssl_test_file -s "bar" 2>&1)"

    check_manual "-r -q -p -s foo fichier" \
        'just to be extra clear
3ba35f1ea0d170cb3b9a752e3360286c
acbd18db4cc2f85cedef654fccc4a4d8
53d53ea94217b259c11a5a2d104ec58a' \
        "$(echo "just to be extra clear" | $FT_SSL md5 -r -q -p -s "foo" /tmp/ft_ssl_test_file)"
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