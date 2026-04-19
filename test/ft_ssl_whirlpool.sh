#!/bin/bash

# ================================================================
# test_ft_ssl_whirlpool.sh — Comparaison ft_ssl whirlpool vs openssl
# Usage : ./test_ft_ssl_whirlpool.sh [chemin vers ft_ssl]
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
    printf 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' \
        > /tmp/ft_ssl_test_63a
    printf 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' \
        > /tmp/ft_ssl_test_64a
    printf 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' \
        > /tmp/ft_ssl_test_65a
    printf 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' \
        > /tmp/ft_ssl_test_192a
}

cleanup()
{
    rm -f /tmp/ft_ssl_test_file
    rm -f /tmp/ft_ssl_test_website
    rm -f /tmp/ft_ssl_test_63a
    rm -f /tmp/ft_ssl_test_64a
    rm -f /tmp/ft_ssl_test_65a
    rm -f /tmp/ft_ssl_test_192a
}

# ================================================================
# WHIRLPOOL STDIN — comparaison directe openssl vs ft_ssl
# ================================================================

test_whirlpool_stdin()
{
    echo "=== WHIRLPOOL STDIN ==="

    check "stdin abc" \
        "$(echo -n "abc" | $FT_SSL whirlpool)" \
        "$(echo -n "abc" | openssl whirlpool)"

    check "stdin vide" \
        "$(echo -n "" | $FT_SSL whirlpool)" \
        "$(echo -n "" | openssl whirlpool)"

    check "stdin avec newline" \
        "$(echo "42 is nice" | $FT_SSL whirlpool)" \
        "$(echo "42 is nice" | openssl whirlpool)"

    check "stdin 63 chars (< 64)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL whirlpool)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl whirlpool)"

    check "stdin 64 chars (bloc plein)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL whirlpool)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl whirlpool)"

    check "stdin 65 chars (bloc + 1)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL whirlpool)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl whirlpool)"

    check "stdin 192 chars (3 blocs)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | $FT_SSL whirlpool)" \
        "$(echo -n "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | openssl whirlpool)"

    check "stdin long message" \
        "$(echo -n "The quick brown fox jumps over the lazy dog" | $FT_SSL whirlpool)" \
        "$(echo -n "The quick brown fox jumps over the lazy dog" | openssl whirlpool)"

    check "stdin quick brown eog" \
        "$(echo -n "The quick brown fox jumps over the lazy eog" | $FT_SSL whirlpool)" \
        "$(echo -n "The quick brown fox jumps over the lazy eog" | openssl whirlpool)"

    check "stdin pity..." \
        "$(echo -n "pity those that aren't following baerista on spotify." | $FT_SSL whirlpool)" \
        "$(echo -n "pity those that aren't following baerista on spotify." | openssl whirlpool)"
}

# ================================================================
# WHIRLPOOL FLAG -s
# ================================================================

test_whirlpool_flag_s()
{
    echo ""
    echo "=== WHIRLPOOL FLAG -s ==="

    check "-s abc" \
        "$($FT_SSL whirlpool -s "abc")" \
        "WHIRLPOOL(\"abc\")= $(echo -n "abc" | openssl whirlpool | awk '{print $2}')"

    check "-s foo" \
        "$($FT_SSL whirlpool -s "foo")" \
        "WHIRLPOOL(\"foo\")= $(echo -n "foo" | openssl whirlpool | awk '{print $2}')"

    check "-s vide" \
        "$($FT_SSL whirlpool -s "")" \
        "WHIRLPOOL(\"\")= $(echo -n "" | openssl whirlpool | awk '{print $2}')"

    check "-s pity..." \
        "$($FT_SSL whirlpool -s "pity those that aren't following baerista on spotify.")" \
        "WHIRLPOOL(\"pity those that aren't following baerista on spotify.\")= $(echo -n "pity those that aren't following baerista on spotify." | openssl whirlpool | awk '{print $2}')"

    check "-s plusieurs fois" \
        "$($FT_SSL whirlpool -s "foo" -s "bar" | tail -1)" \
        "WHIRLPOOL(\"bar\")= $(echo -n "bar" | openssl whirlpool | awk '{print $2}')"
}

# ================================================================
# WHIRLPOOL FLAG -q
# ================================================================

test_whirlpool_flag_q()
{
    echo ""
    echo "=== WHIRLPOOL FLAG -q ==="

    check "-q stdin abc" \
        "$(echo -n "abc" | $FT_SSL whirlpool -q)" \
        "$(echo -n "abc" | openssl whirlpool | awk '{print $2}')"

    check "-q stdin vide" \
        "$(echo -n "" | $FT_SSL whirlpool -q)" \
        "$(echo -n "" | openssl whirlpool | awk '{print $2}')"

    check "-q fichier" \
        "$($FT_SSL whirlpool -q /tmp/ft_ssl_test_file)" \
        "$(openssl whirlpool /tmp/ft_ssl_test_file | awk '{print $2}')"

    check "-q fichier 64 chars" \
        "$($FT_SSL whirlpool -q /tmp/ft_ssl_test_64a)" \
        "$(openssl whirlpool /tmp/ft_ssl_test_64a | awk '{print $2}')"

    check "-q -s abc" \
        "$($FT_SSL whirlpool -q -s "abc")" \
        "$(echo -n "abc" | openssl whirlpool | awk '{print $2}')"
}

# ================================================================
# WHIRLPOOL FLAG -r
# ================================================================

test_whirlpool_flag_r()
{
    echo ""
    echo "=== WHIRLPOOL FLAG -r ==="

    check "-r fichier" \
        "$($FT_SSL whirlpool -r /tmp/ft_ssl_test_file)" \
        "$(openssl whirlpool /tmp/ft_ssl_test_file | awk '{print $2}') /tmp/ft_ssl_test_file"

    check "-r -s foo" \
        "$($FT_SSL whirlpool -r -s "foo")" \
        "$(echo -n "foo" | openssl whirlpool | awk '{print $2}') \"foo\""

    check "-r stdin" \
        "$(echo -n "abc" | $FT_SSL whirlpool -r)" \
        "$(echo -n "abc" | openssl whirlpool | awk '{print $2}') (stdin)"
}

# ================================================================
# WHIRLPOOL FICHIERS — comparaison directe openssl vs ft_ssl
# ================================================================

test_whirlpool_files()
{
    echo ""
    echo "=== WHIRLPOOL FICHIERS ==="

    check "fichier test_file" \
        "$($FT_SSL whirlpool /tmp/ft_ssl_test_file)" \
        "$(openssl whirlpool /tmp/ft_ssl_test_file)"

    check "fichier test_website" \
        "$($FT_SSL whirlpool /tmp/ft_ssl_test_website)" \
        "$(openssl whirlpool /tmp/ft_ssl_test_website)"

    check "fichier 63 chars" \
        "$($FT_SSL whirlpool /tmp/ft_ssl_test_63a)" \
        "$(openssl whirlpool /tmp/ft_ssl_test_63a)"

    check "fichier 64 chars" \
        "$($FT_SSL whirlpool /tmp/ft_ssl_test_64a)" \
        "$(openssl whirlpool /tmp/ft_ssl_test_64a)"

    check "fichier 65 chars" \
        "$($FT_SSL whirlpool /tmp/ft_ssl_test_65a)" \
        "$(openssl whirlpool /tmp/ft_ssl_test_65a)"

    check "fichier 192 chars" \
        "$($FT_SSL whirlpool /tmp/ft_ssl_test_192a)" \
        "$(openssl whirlpool /tmp/ft_ssl_test_192a)"

    check_manual "fichier inexistant" \
        "ft_ssl: whirlpool: /tmp/inexistant: No such file or directory" \
        "$($FT_SSL whirlpool /tmp/inexistant 2>&1)"

    check "plusieurs fichiers nb lignes" \
        "$($FT_SSL whirlpool /tmp/ft_ssl_test_file /tmp/ft_ssl_test_website | wc -l)" \
        "$(openssl whirlpool /tmp/ft_ssl_test_file /tmp/ft_ssl_test_website | wc -l)"
}

# ================================================================
# WHIRLPOOL FLAG -p
# ================================================================

test_whirlpool_flag_p()
{
    echo ""
    echo "=== WHIRLPOOL FLAG -p ==="

    # Test que -p affiche bien 2 lignes
    check "-p abc nb lignes" \
        "$(echo -n "abc" | $FT_SSL whirlpool -p | wc -l)" \
        "2"

    # Test que la première ligne est bien le contenu
    check "-p abc ligne 1" \
        "$(echo -n "abc" | $FT_SSL whirlpool -p | sed -n '1p')" \
        "abc"

    # Test que la deuxième ligne contient le bon hash
    check "-p abc hash" \
        "$(echo -n "abc" | $FT_SSL whirlpool -p | sed -n '2p')" \
        "WHIRLPOOL(stdin)= $(echo -n "abc" | openssl whirlpool | awk '{print $2}')"

    # Test avec newline
    check "-p avec newline nb lignes" \
        "$(echo "42 is nice" | $FT_SSL whirlpool -p | wc -l)" \
        "2"

    check "-p avec newline ligne 1" \
        "$(echo "42 is nice" | $FT_SSL whirlpool -p | sed -n '1p')" \
        "42 is nice"

    check "-p avec newline hash" \
        "$(echo "42 is nice" | $FT_SSL whirlpool -p | sed -n '2p')" \
        "WHIRLPOOL(stdin)= $(echo "42 is nice" | openssl whirlpool | awk '{print $2}')"

    # Test -p avec fichier
    check "-p + fichier nb lignes" \
        "$(echo "be sure" | $FT_SSL whirlpool -p /tmp/ft_ssl_test_file | wc -l)" \
        "3"

    check "-p + fichier hash stdin" \
        "$(echo "be sure" | $FT_SSL whirlpool -p /tmp/ft_ssl_test_file | sed -n '2p')" \
        "WHIRLPOOL(stdin)= $(echo "be sure" | openssl whirlpool | awk '{print $2}')"

    check "-p + fichier hash fichier" \
        "$(echo "be sure" | $FT_SSL whirlpool -p /tmp/ft_ssl_test_file | tail -1)" \
        "$(openssl whirlpool /tmp/ft_ssl_test_file)"
}

# ================================================================
# WHIRLPOOL COMBINAISONS
# ================================================================

test_whirlpool_combinations()
{
    echo ""
    echo "=== WHIRLPOOL COMBINAISONS ==="

    check "-p -r nb lignes" \
        "$(echo "but eventually" | $FT_SSL whirlpool -p -r /tmp/ft_ssl_test_file | wc -l)" \
        "3"

    check "-p -r stdin hash" \
        "$(echo "but eventually" | $FT_SSL whirlpool -p -r /tmp/ft_ssl_test_file | sed -n '2p')" \
        "$(echo "but eventually" | openssl whirlpool | awk '{print $2}') (stdin)"

    check "-p -r fichier hash" \
        "$(echo "but eventually" | $FT_SSL whirlpool -p -r /tmp/ft_ssl_test_file | tail -1)" \
        "$(openssl whirlpool /tmp/ft_ssl_test_file | awk '{print $2}') /tmp/ft_ssl_test_file"

    check "-p -s foo nb lignes" \
        "$(echo "GL HF" | $FT_SSL whirlpool -p -s "foo" /tmp/ft_ssl_test_file | wc -l)" \
        "4"

    check "-p -s foo stdin hash" \
        "$(echo "GL HF" | $FT_SSL whirlpool -p -s "foo" /tmp/ft_ssl_test_file | sed -n '2p')" \
        "WHIRLPOOL(stdin)= $(echo "GL HF" | openssl whirlpool | awk '{print $2}')"

    check "-p -s foo string hash" \
        "$(echo "GL HF" | $FT_SSL whirlpool -p -s "foo" /tmp/ft_ssl_test_file | sed -n '3p')" \
        "WHIRLPOOL(\"foo\")= $(echo -n "foo" | openssl whirlpool | awk '{print $2}')"

    check "-q -p nb lignes" \
        "$(echo -n "just to be extra clear" | $FT_SSL whirlpool -q -p | wc -l)" \
        "2"

    check "-q -p ligne 1" \
        "$(echo -n "just to be extra clear" | $FT_SSL whirlpool -q -p | sed -n '1p')" \
        "just to be extra clear"

    check "-q -p hash" \
        "$(echo -n "just to be extra clear" | $FT_SSL whirlpool -q -p | tail -1)" \
        "$(echo -n "just to be extra clear" | openssl whirlpool | awk '{print $2}')"

    check "-r -q stdin" \
        "$(echo -n "abc" | $FT_SSL whirlpool -r -q)" \
        "$(echo -n "abc" | openssl whirlpool | awk '{print $2}')"

    check "-r -q -s foo" \
        "$($FT_SSL whirlpool -r -q -s "foo")" \
        "$(echo -n "foo" | openssl whirlpool | awk '{print $2}')"
}

# ================================================================
# WHIRLPOOL EDGE CASES
# ================================================================

test_whirlpool_edge_cases()
{
    echo ""
    echo "=== WHIRLPOOL EDGE CASES ==="

    check "message 1 byte" \
        "$(echo -n "a" | $FT_SSL whirlpool -q)" \
        "$(echo -n "a" | openssl whirlpool | awk '{print $2}')"

    check "message 2 bytes" \
        "$(echo -n "ab" | $FT_SSL whirlpool -q)" \
        "$(echo -n "ab" | openssl whirlpool | awk '{print $2}')"

    check "message 31 bytes" \
        "$(python3 -c "print('a'*31, end='')" | $FT_SSL whirlpool -q)" \
        "$(python3 -c "print('a'*31, end='')" | openssl whirlpool | awk '{print $2}')"

    check "message 32 bytes" \
        "$(python3 -c "print('a'*32, end='')" | $FT_SSL whirlpool -q)" \
        "$(python3 -c "print('a'*32, end='')" | openssl whirlpool | awk '{print $2}')"

    check "message 33 bytes" \
        "$(python3 -c "print('a'*33, end='')" | $FT_SSL whirlpool -q)" \
        "$(python3 -c "print('a'*33, end='')" | openssl whirlpool | awk '{print $2}')"

    check "message 127 bytes" \
        "$(python3 -c "print('a'*127, end='')" | $FT_SSL whirlpool -q)" \
        "$(python3 -c "print('a'*127, end='')" | openssl whirlpool | awk '{print $2}')"

    check "message 128 bytes" \
        "$(python3 -c "print('a'*128, end='')" | $FT_SSL whirlpool -q)" \
        "$(python3 -c "print('a'*128, end='')" | openssl whirlpool | awk '{print $2}')"

    check "message 129 bytes" \
        "$(python3 -c "print('a'*129, end='')" | $FT_SSL whirlpool -q)" \
        "$(python3 -c "print('a'*129, end='')" | openssl whirlpool | awk '{print $2}')"

    check "caractères spéciaux" \
        "$(echo -n "!@#$%^&*()_+-=[]{}|;:,.<>?" | $FT_SSL whirlpool -q)" \
        "$(echo -n "!@#$%^&*()_+-=[]{}|;:,.<>?" | openssl whirlpool | awk '{print $2}')"

    check "unicode basique" \
        "$(echo -n "café" | $FT_SSL whirlpool -q)" \
        "$(echo -n "café" | openssl whirlpool | awk '{print $2}')"
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
    
    if [ $FAIL -eq 0 ]; then
        echo -e "${GREEN}✓ Tous les tests passent !${RESET}"
    else
        echo -e "${RED}✗ $FAIL test(s) échoué(s)${RESET}"
    fi
}

# ================================================================
# main
# ================================================================

setup
test_whirlpool_stdin
test_whirlpool_flag_s
test_whirlpool_flag_q
test_whirlpool_flag_r
test_whirlpool_files
test_whirlpool_flag_p
test_whirlpool_combinations
test_whirlpool_edge_cases
summary
cleanup

exit $FAIL