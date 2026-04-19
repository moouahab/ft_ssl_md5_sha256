#!/bin/bash

# ================================================================
# test_valgrind.sh — Vérification des fuites mémoire avec Valgrind
# Usage : ./test_valgrind.sh [chemin vers ft_ssl]
# ================================================================

FT_SSL=${1:-"../ft_ssl"}
PASS=0
FAIL=0
TOTAL=0

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
RESET='\033[0m'

# Vérifier que valgrind est installé
if ! command -v valgrind &> /dev/null; then
    echo -e "${RED}❌ Valgrind n'est pas installé${RESET}"
    echo "Installation:"
    echo "  Ubuntu/Debian: sudo apt-get install valgrind"
    echo "  macOS: brew install valgrind (peut ne pas fonctionner sur M1/M2)"
    exit 1
fi

# Vérifier que ft_ssl existe
if [ ! -f "$FT_SSL" ]; then
    echo -e "${RED}❌ ft_ssl introuvable: $FT_SSL${RESET}"
    exit 1
fi

echo -e "${YELLOW}═══════════════════════════════════════════════════${RESET}"
echo -e "${YELLOW}  TEST VALGRIND - FT_SSL                          ${RESET}"
echo -e "${YELLOW}═══════════════════════════════════════════════════${RESET}"
echo ""

# ================================================================
# Fonction de test valgrind
# ================================================================

check_valgrind()
{
    local label="$1"
    local cmd="$2"
    
    TOTAL=$((TOTAL + 1))
    
    # Exécuter valgrind et capturer la sortie
    local valgrind_output=$(eval "valgrind --leak-check=full --error-exitcode=1 --quiet $cmd 2>&1")
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        echo -e "${GREEN}[OK]${RESET} $label"
        PASS=$((PASS + 1))
    else
        echo -e "${RED}[KO]${RESET} $label"
        echo "$valgrind_output" | sed 's/^/     /'
        FAIL=$((FAIL + 1))
    fi
}

# ================================================================
# Fichiers de test
# ================================================================

setup()
{
    echo "And above all," > /tmp/ft_ssl_test_file
    echo "https://www.42.fr/" > /tmp/ft_ssl_test_website
    printf 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' \
        > /tmp/ft_ssl_test_64a
    printf 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' \
        > /tmp/ft_ssl_test_128a
}

cleanup()
{
    rm -f /tmp/ft_ssl_test_file
    rm -f /tmp/ft_ssl_test_website
    rm -f /tmp/ft_ssl_test_64a
    rm -f /tmp/ft_ssl_test_128a
}

# ================================================================
# TESTS MD5
# ================================================================

test_md5_valgrind()
{
    echo "=== MD5 VALGRIND ==="
    
    check_valgrind "md5 stdin simple" \
        "echo -n 'abc' | $FT_SSL md5"
    
    check_valgrind "md5 stdin vide" \
        "echo -n '' | $FT_SSL md5"
    
    check_valgrind "md5 stdin long" \
        "echo -n 'The quick brown fox jumps over the lazy dog' | $FT_SSL md5"
    
    check_valgrind "md5 fichier" \
        "$FT_SSL md5 /tmp/ft_ssl_test_file"
    
    check_valgrind "md5 plusieurs fichiers" \
        "$FT_SSL md5 /tmp/ft_ssl_test_file /tmp/ft_ssl_test_website"
    
    check_valgrind "md5 -s" \
        "$FT_SSL md5 -s 'hello world'"
    
    check_valgrind "md5 -p" \
        "echo -n 'test' | $FT_SSL md5 -p"
    
    check_valgrind "md5 -q" \
        "echo -n 'test' | $FT_SSL md5 -q"
    
    check_valgrind "md5 -r" \
        "$FT_SSL md5 -r /tmp/ft_ssl_test_file"
    
    check_valgrind "md5 -p -s -r combinés" \
        "echo 'stdin' | $FT_SSL md5 -p -s 'string' -r /tmp/ft_ssl_test_file"
    
    check_valgrind "md5 fichier inexistant" \
        "$FT_SSL md5 /tmp/fichier_inexistant"
}

# ================================================================
# TESTS SHA256
# ================================================================

test_sha256_valgrind()
{
    echo ""
    echo "=== SHA256 VALGRIND ==="
    
    check_valgrind "sha256 stdin simple" \
        "echo -n 'abc' | $FT_SSL sha256"
    
    check_valgrind "sha256 stdin vide" \
        "echo -n '' | $FT_SSL sha256"
    
    check_valgrind "sha256 stdin 55 chars" \
        "echo -n 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' | $FT_SSL sha256"
    
    check_valgrind "sha256 stdin 56 chars" \
        "echo -n 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' | $FT_SSL sha256"
    
    check_valgrind "sha256 fichier" \
        "$FT_SSL sha256 /tmp/ft_ssl_test_file"
    
    check_valgrind "sha256 fichier 64 chars" \
        "$FT_SSL sha256 /tmp/ft_ssl_test_64a"
    
    check_valgrind "sha256 plusieurs fichiers" \
        "$FT_SSL sha256 /tmp/ft_ssl_test_file /tmp/ft_ssl_test_website"
    
    check_valgrind "sha256 -s" \
        "$FT_SSL sha256 -s 'hello world'"
    
    check_valgrind "sha256 -p" \
        "echo -n 'test' | $FT_SSL sha256 -p"
    
    check_valgrind "sha256 -q" \
        "echo -n 'test' | $FT_SSL sha256 -q"
    
    check_valgrind "sha256 -r" \
        "$FT_SSL sha256 -r /tmp/ft_ssl_test_file"
    
    check_valgrind "sha256 -p -s -r combinés" \
        "echo 'stdin' | $FT_SSL sha256 -p -s 'string' -r /tmp/ft_ssl_test_file"
}

# ================================================================
# TESTS SHA512
# ================================================================

test_sha512_valgrind()
{
    echo ""
    echo "=== SHA512 VALGRIND ==="
    
    check_valgrind "sha512 stdin simple" \
        "echo -n 'abc' | $FT_SSL sha512"
    
    check_valgrind "sha512 stdin vide" \
        "echo -n '' | $FT_SSL sha512"
    
    check_valgrind "sha512 stdin 111 chars" \
        "echo -n 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' | $FT_SSL sha512"
    
    check_valgrind "sha512 stdin 112 chars" \
        "echo -n 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' | $FT_SSL sha512"
    
    check_valgrind "sha512 fichier" \
        "$FT_SSL sha512 /tmp/ft_ssl_test_file"
    
    check_valgrind "sha512 fichier 128 chars" \
        "$FT_SSL sha512 /tmp/ft_ssl_test_128a"
    
    check_valgrind "sha512 plusieurs fichiers" \
        "$FT_SSL sha512 /tmp/ft_ssl_test_file /tmp/ft_ssl_test_website"
    
    check_valgrind "sha512 -s" \
        "$FT_SSL sha512 -s 'hello world'"
    
    check_valgrind "sha512 -p" \
        "echo -n 'test' | $FT_SSL sha512 -p"
    
    check_valgrind "sha512 -q" \
        "echo -n 'test' | $FT_SSL sha512 -q"
    
    check_valgrind "sha512 -r" \
        "$FT_SSL sha512 -r /tmp/ft_ssl_test_file"
    
    check_valgrind "sha512 -p -s -r combinés" \
        "echo 'stdin' | $FT_SSL sha512 -p -s 'string' -r /tmp/ft_ssl_test_file"
}

# ================================================================
# TESTS WHIRLPOOL
# ================================================================

test_whirlpool_valgrind()
{
    echo ""
    echo "=== WHIRLPOOL VALGRIND ==="
    
    check_valgrind "whirlpool stdin simple" \
        "echo -n 'abc' | $FT_SSL whirlpool"
    
    check_valgrind "whirlpool stdin vide" \
        "echo -n '' | $FT_SSL whirlpool"
    
    check_valgrind "whirlpool stdin 63 chars" \
        "echo -n 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' | $FT_SSL whirlpool"
    
    check_valgrind "whirlpool stdin 64 chars" \
        "echo -n 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' | $FT_SSL whirlpool"
    
    check_valgrind "whirlpool fichier" \
        "$FT_SSL whirlpool /tmp/ft_ssl_test_file"
    
    check_valgrind "whirlpool fichier 64 chars" \
        "$FT_SSL whirlpool /tmp/ft_ssl_test_64a"
    
    check_valgrind "whirlpool plusieurs fichiers" \
        "$FT_SSL whirlpool /tmp/ft_ssl_test_file /tmp/ft_ssl_test_website"
    
    check_valgrind "whirlpool -s" \
        "$FT_SSL whirlpool -s 'hello world'"
    
    check_valgrind "whirlpool -p" \
        "echo -n 'test' | $FT_SSL whirlpool -p"
    
    check_valgrind "whirlpool -q" \
        "echo -n 'test' | $FT_SSL whirlpool -q"
    
    check_valgrind "whirlpool -r" \
        "$FT_SSL whirlpool -r /tmp/ft_ssl_test_file"
    
    check_valgrind "whirlpool -p -s -r combinés" \
        "echo 'stdin' | $FT_SSL whirlpool -p -s 'string' -r /tmp/ft_ssl_test_file"
}

# ================================================================
# TESTS D'ERREURS
# ================================================================

test_errors_valgrind()
{
    echo ""
    echo "=== ERREURS VALGRIND ==="
    
    check_valgrind "commande invalide" \
        "$FT_SSL invalid_command"
    
    check_valgrind "sans arguments" \
        "$FT_SSL"
    
    check_valgrind "fichier inexistant" \
        "$FT_SSL md5 /tmp/fichier_qui_nexiste_pas"
}

# ================================================================
# TESTS DE STRESS
# ================================================================

test_stress_valgrind()
{
    echo ""
    echo "=== STRESS TESTS VALGRIND ==="
    
    check_valgrind "md5 message très long" \
        "python3 -c \"print('a'*10000, end='')\" | $FT_SSL md5"
    
    check_valgrind "sha256 message très long" \
        "python3 -c \"print('a'*10000, end='')\" | $FT_SSL sha256"
    
    check_valgrind "sha512 message très long" \
        "python3 -c \"print('a'*10000, end='')\" | $FT_SSL sha512"
    
    check_valgrind "whirlpool message très long" \
        "python3 -c \"print('a'*10000, end='')\" | $FT_SSL whirlpool"
    
    check_valgrind "md5 10 fichiers" \
        "$FT_SSL md5 /tmp/ft_ssl_test_file /tmp/ft_ssl_test_file /tmp/ft_ssl_test_file /tmp/ft_ssl_test_file /tmp/ft_ssl_test_file /tmp/ft_ssl_test_file /tmp/ft_ssl_test_file /tmp/ft_ssl_test_file /tmp/ft_ssl_test_file /tmp/ft_ssl_test_file"
    
    check_valgrind "sha256 multiples -s" \
        "$FT_SSL sha256 -s 'test1' -s 'test2' -s 'test3' -s 'test4' -s 'test5'"
}

# ================================================================
# RÉSUMÉ
# ================================================================

summary()
{
    echo ""
    echo -e "${YELLOW}═══════════════════════════════════════════════════${RESET}"
    echo "TOTAL  : $TOTAL"
    echo -e "PASS   : ${GREEN}$PASS${RESET}"
    if [ $FAIL -gt 0 ]; then
        echo -e "FAIL   : ${RED}$FAIL${RESET}"
    else
        echo "FAIL   : $FAIL"
    fi
    echo -e "${YELLOW}═══════════════════════════════════════════════════${RESET}"
    
    if [ $FAIL -eq 0 ]; then
        echo -e "${GREEN}✓ Aucune fuite mémoire détectée !${RESET}"
        echo -e "${GREEN}✓ Tous les tests Valgrind passent !${RESET}"
    else
        echo -e "${RED}✗ $FAIL fuite(s) mémoire détectée(s)${RESET}"
        echo -e "${YELLOW}⚠ Vérifiez les messages d'erreur ci-dessus${RESET}"
    fi
}

# ================================================================
# MAIN
# ================================================================

setup
test_md5_valgrind
test_sha256_valgrind
test_sha512_valgrind
test_whirlpool_valgrind
test_errors_valgrind
test_stress_valgrind
summary
cleanup

exit $FAIL