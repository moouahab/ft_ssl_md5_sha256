#!/bin/bash

# ================================================================
# test_ft_ssl.sh — Tests automatisés pour ft_ssl md5
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
    local expected="$2"
    local got="$3"

    TOTAL=$((TOTAL + 1))
    if [ "$expected" = "$got" ]; then
        echo -e "${GREEN}[OK]${RESET} $label"
        PASS=$((PASS + 1))
    else
        echo -e "${RED}[KO]${RESET} $label"
        echo "     attendu : $expected"
        echo "     obtenu  : $got"
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
# tests stdin
# ================================================================

test_stdin()
{
    echo "=== STDIN ==="

    check "stdin abc" \
        "MD5(stdin)= 900150983cd24fb0d6963f7d28e17f72" \
        "$(echo -n "abc" | $FT_SSL md5)"

    check "stdin 42 is nice" \
        "MD5(stdin)= 35f1d6de0302e2086a4e472266efb3a9" \
        "$(echo "42 is nice" | $FT_SSL md5)"

    check "stdin vide" \
        "MD5(stdin)= d41d8cd98f00b204e9800998ecf8427e" \
        "$(echo -n "" | $FT_SSL md5)"
}

# ================================================================
# tests flag -s
# ================================================================

test_flag_s()
{
    echo ""
    echo "=== FLAG -s ==="

    check "-s abc" \
        'MD5("abc")= 900150983cd24fb0d6963f7d28e17f72' \
        "$($FT_SSL md5 -s "abc")"

    check "-s foo" \
        'MD5("foo")= acbd18db4cc2f85cedef654fccc4a4d8' \
        "$($FT_SSL md5 -s "foo")"

    check "-s pity..." \
        'MD5("pity those that aren'"'"'t following baerista on spotify.")= a3c990a1964705d9bf0e602f44572f5f' \
        "$($FT_SSL md5 -s "pity those that aren't following baerista on spotify.")"
}

# ================================================================
# tests flag -q
# ================================================================

test_flag_q()
{
    echo ""
    echo "=== FLAG -q ==="

    check "-q stdin" \
        "900150983cd24fb0d6963f7d28e17f72" \
        "$(echo -n "abc" | $FT_SSL md5 -q)"

    check "-q -s foo" \
        "acbd18db4cc2f85cedef654fccc4a4d8" \
        "$($FT_SSL md5 -q -s "foo")"

    check "-q -r stdin" \
        "e20c3b973f63482a778f3fd1869b7f25" \
        "$(echo "Pity the living." | $FT_SSL md5 -q -r)"
}

# ================================================================
# tests flag -r
# ================================================================

test_flag_r()
{
    echo ""
    echo "=== FLAG -r ==="

    check "-r fichier" \
        "53d53ea94217b259c11a5a2d104ec58a /tmp/ft_ssl_test_file" \
        "$($FT_SSL md5 -r /tmp/ft_ssl_test_file)"

    check "-r -s foo" \
        'acbd18db4cc2f85cedef654fccc4a4d8 "foo"' \
        "$($FT_SSL md5 -r -s "foo")"
}

# ================================================================
# tests fichiers
# ================================================================

test_files()
{
    echo ""
    echo "=== FICHIERS ==="

    check "fichier normal" \
        "MD5(/tmp/ft_ssl_test_file)= 53d53ea94217b259c11a5a2d104ec58a" \
        "$($FT_SSL md5 /tmp/ft_ssl_test_file)"

    check "fichier inexistant" \
        "ft_ssl: md5: /tmp/inexistant: No such file or directory" \
        "$($FT_SSL md5 /tmp/inexistant 2>&1)"
}

# ================================================================
# tests flag -p
# ================================================================

test_flag_p()
{
    echo ""
    echo "=== FLAG -p ==="

    check "-p abc sans newline" \
        '("abc")= 900150983cd24fb0d6963f7d28e17f72' \
        "$(echo -n "abc" | $FT_SSL md5 -p)"

    check "-p 42 is nice avec newline" \
        '("42 is nice\n")= 35f1d6de0302e2086a4e472266efb3a9' \
        "$(echo "42 is nice" | $FT_SSL md5 -p)"

    check "-p + fichier" \
        '("be sure to handle edge cases carefully")= 3553dc7dc5963b583c056d1b9fa3349c
MD5(/tmp/ft_ssl_test_file)= 53d53ea94217b259c11a5a2d104ec58a' \
        "$(echo -n "be sure to handle edge cases carefully" | $FT_SSL md5 -p /tmp/ft_ssl_test_file)"
}

# ================================================================
# tests combinaisons
# ================================================================

test_combinations()
{
    echo ""
    echo "=== COMBINAISONS ==="

    check "-p -r fichier" \
        '("but eventually you will understand")= dcdd84e0f635694d2a943fa8d3905281
53d53ea94217b259c11a5a2d104ec58a /tmp/ft_ssl_test_file' \
        "$(echo -n "but eventually you will understand" | $FT_SSL md5 -p -r /tmp/ft_ssl_test_file)"

    check "-p -s foo fichier" \
        '("GL HF let'"'"'s go")= d1e3cc342b6da09480b27ec57ff243e2
MD5("foo")= acbd18db4cc2f85cedef654fccc4a4d8
MD5(/tmp/ft_ssl_test_file)= 53d53ea94217b259c11a5a2d104ec58a' \
        "$(echo -n "GL HF let's go" | $FT_SSL md5 -p -s "foo" /tmp/ft_ssl_test_file)"

    check "-r -p -s foo fichier -s bar" \
        '("one more thing")= a0bd1876c6f011dd50fae52827f445f5
acbd18db4cc2f85cedef654fccc4a4d8 "foo"
53d53ea94217b259c11a5a2d104ec58a /tmp/ft_ssl_test_file
ft_ssl: md5: -s: No such file or directory
ft_ssl: md5: bar: No such file or directory' \
        "$(echo -n "one more thing" | $FT_SSL md5 -r -p -s "foo" /tmp/ft_ssl_test_file -s "bar" 2>&1)"

    check "-r -q -p -s foo fichier" \
        'just to be extra clear
3ba35f1ea0d170cb3b9a752e3360286c
acbd18db4cc2f85cedef654fccc4a4d8
53d53ea94217b259c11a5a2d104ec58a' \
        "$(echo -n "just to be extra clear" | $FT_SSL md5 -r -q -p -s "foo" /tmp/ft_ssl_test_file)"
}

# ================================================================
# tests erreurs
# ================================================================

test_errors()
{
    echo ""
    echo "=== ERREURS ==="

    check "no args" \
        "usage: ft_ssl command [flags] [file/string]" \
        "$($FT_SSL 2>&1)"

    check "commande invalide" \
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
        echo -e "FAIL   : $FAIL"
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