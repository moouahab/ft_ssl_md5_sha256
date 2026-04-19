#!/bin/bash

# ================================================================
# run_all_test.sh — Lance tous les tests (fonctionnels + valgrind)
# Usage : ./run_all_test.sh [chemin vers ft_ssl]
# ================================================================

FT_SSL=${1:-"../ft_ssl"}

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
RESET='\033[0m'

# Vérifier que ft_ssl existe
if [ ! -f "$FT_SSL" ]; then
    echo -e "${RED}❌ ft_ssl introuvable: $FT_SSL${RESET}"
    exit 1
fi

echo -e "${BLUE}╔═══════════════════════════════════════════════════╗${RESET}"
echo -e "${BLUE}║                                                   ║${RESET}"
echo -e "${BLUE}║           FT_SSL - SUITE DE TESTS COMPLÈTE        ║${RESET}"
echo -e "${BLUE}║                                                   ║${RESET}"
echo -e "${BLUE}╚═══════════════════════════════════════════════════╝${RESET}"
echo ""

TOTAL_TESTS=0
TOTAL_PASS=0
TOTAL_FAIL=0

# ================================================================
# TESTS FONCTIONNELS
# ================================================================

echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"
echo -e "${YELLOW}  PHASE 1 : TESTS FONCTIONNELS                     ${RESET}"
echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"
echo ""

run_test_script()
{
    local script=$1
    local algo=$2
    
    if [ ! -f "$script" ]; then
        echo -e "${RED}❌ Script introuvable: $script${RESET}"
        return 1
    fi
    
    echo -e "${BLUE}┌─ $algo${RESET}"
    
    # Exécuter le script dans un fichier temporaire
    local tmpfile=$(mktemp)
    $script $FT_SSL > "$tmpfile" 2>&1
    local exit_code=$?
    
    # Lire le contenu et SUPPRIMER LES CODES COULEUR ANSI
    local output=$(cat "$tmpfile" | sed 's/\x1b\[[0-9;]*m//g')
    
    # Extraire PASS et TOTAL (plusieurs formats possibles)
    local pass=""
    local total=""
    
    # Format 1: "PASS   : 43" (sans ^ car peut y avoir des espaces)
    if echo "$output" | grep -q "PASS"; then
        pass=$(echo "$output" | grep "PASS" | grep -oE '[0-9]+' | head -1)
    fi
    
    # Format 2: "TOTAL  : 43" (sans ^ car peut y avoir des espaces)
    if echo "$output" | grep -q "TOTAL"; then
        total=$(echo "$output" | grep "TOTAL" | grep -oE '[0-9]+' | head -1)
    fi
    
    # Valeurs par défaut
    pass=${pass:-0}
    total=${total:-0}
    
    # Calculer les échecs
    local fail=0
    if [ "$total" -gt 0 ]; then
        fail=$((total - pass))
    fi
    
    # Mettre à jour les totaux
    TOTAL_TESTS=$((TOTAL_TESTS + total))
    TOTAL_PASS=$((TOTAL_PASS + pass))
    TOTAL_FAIL=$((TOTAL_FAIL + fail))
    
    # Afficher le résultat
    if [ $fail -eq 0 ] && [ $total -gt 0 ]; then
        echo -e "${GREEN}└─ ✓ $pass/$total tests${RESET}"
    elif [ $total -eq 0 ]; then
        echo -e "${YELLOW}└─ ⚠ Aucun test trouvé${RESET}"
        cat "$tmpfile" | head -20
    else
        echo -e "${RED}└─ ✗ $pass/$total tests ($fail échecs)${RESET}"
        echo "$output" | grep "\[KO\]" | head -5 | sed 's/^/   /'
    fi
    
    rm -f "$tmpfile"
    return $exit_code
}

# Lancer chaque suite de tests
run_test_script "./ft_ssl_md5.sh" "MD5"
run_test_script "./ft_ssl_sha256.sh" "SHA256"
run_test_script "./ft_ssl_sha512.sh" "SHA512"
run_test_script "./ft_ssl_whirlpool.sh" "WHIRLPOOL"

echo ""
echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"
echo -e "${YELLOW}  Résumé Tests Fonctionnels                        ${RESET}"
echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"
echo "Total  : $TOTAL_TESTS tests"
if [ $TOTAL_FAIL -eq 0 ] && [ $TOTAL_TESTS -gt 0 ]; then
    echo -e "Pass   : ${GREEN}$TOTAL_PASS${RESET}"
    echo -e "Fail   : $TOTAL_FAIL"
    echo -e "${GREEN}✓ Tous les tests fonctionnels passent !${RESET}"
elif [ $TOTAL_TESTS -eq 0 ]; then
    echo -e "${YELLOW}⚠ Aucun test fonctionnel trouvé${RESET}"
    echo -e "${YELLOW}⚠ Vérifiez que les scripts de test sont présents${RESET}"
else
    echo -e "Pass   : ${YELLOW}$TOTAL_PASS${RESET}"
    echo -e "Fail   : ${RED}$TOTAL_FAIL${RESET}"
    echo -e "${RED}✗ Certains tests fonctionnels échouent${RESET}"
fi
echo ""

# ================================================================
# TESTS VALGRIND (optionnel)
# ================================================================

if command -v valgrind &> /dev/null; then
    echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"
    echo -e "${YELLOW}  PHASE 2 : TESTS VALGRIND (Fuites mémoire)       ${RESET}"
    echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"
    echo ""
    
    if [ -f "./valgrind.sh" ]; then
        ./valgrind.sh $FT_SSL
        VALGRIND_EXIT=$?
    else
        echo -e "${RED}❌ Script valgrind.sh introuvable${RESET}"
        VALGRIND_EXIT=1
    fi
else
    echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"
    echo -e "${YELLOW}  PHASE 2 : TESTS VALGRIND - SAUTÉS                ${RESET}"
    echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"
    echo ""
    echo -e "${YELLOW}⚠ Valgrind non installé - tests de fuites mémoire sautés${RESET}"
    echo "Pour installer Valgrind:"
    echo "  Ubuntu/Debian: sudo apt-get install valgrind"
    echo "  macOS: brew install valgrind"
    VALGRIND_EXIT=0
fi

# ================================================================
# RÉSUMÉ FINAL
# ================================================================

echo ""
echo -e "${BLUE}╔═══════════════════════════════════════════════════╗${RESET}"
echo -e "${BLUE}║                                                   ║${RESET}"
echo -e "${BLUE}║              RÉSUMÉ FINAL                         ║${RESET}"
echo -e "${BLUE}║                                                   ║${RESET}"
echo -e "${BLUE}╚═══════════════════════════════════════════════════╝${RESET}"
echo ""

if [ $TOTAL_TESTS -gt 0 ]; then
    if [ $TOTAL_FAIL -eq 0 ]; then
        echo -e "${GREEN}✓ Tests Fonctionnels : $TOTAL_PASS/$TOTAL_TESTS${RESET}"
    else
        echo -e "${RED}✗ Tests Fonctionnels : $TOTAL_PASS/$TOTAL_TESTS ($TOTAL_FAIL échecs)${RESET}"
    fi
else
    echo -e "${YELLOW}⚠ Tests Fonctionnels : Aucun test exécuté${RESET}"
fi

if command -v valgrind &> /dev/null; then
    if [ $VALGRIND_EXIT -eq 0 ]; then
        echo -e "${GREEN}✓ Tests Valgrind     : Aucune fuite détectée${RESET}"
    else
        echo -e "${RED}✗ Tests Valgrind     : Fuites mémoire détectées${RESET}"
    fi
else
    echo -e "${YELLOW}⚠ Tests Valgrind     : Non exécutés${RESET}"
fi

echo ""

# Exit code global
if [ $TOTAL_FAIL -eq 0 ] && [ $VALGRIND_EXIT -eq 0 ] && [ $TOTAL_TESTS -gt 0 ]; then
    echo -e "${GREEN}╔═══════════════════════════════════════════════════╗${RESET}"
    echo -e "${GREEN}║                                                   ║${RESET}"
    echo -e "${GREEN}║        ✓ TOUS LES TESTS PASSENT ! 🎉             ║${RESET}"
    echo -e "${GREEN}║                                                   ║${RESET}"
    echo -e "${GREEN}╚═══════════════════════════════════════════════════╝${RESET}"
    exit 0
else
    echo -e "${RED}╔═══════════════════════════════════════════════════╗${RESET}"
    echo -e "${RED}║                                                   ║${RESET}"
    echo -e "${RED}║        ✗ CERTAINS TESTS ÉCHOUENT                 ║${RESET}"
    echo -e "${RED}║                                                   ║${RESET}"
    echo -e "${RED}╚═══════════════════════════════════════════════════╝${RESET}"
    exit 1
fi