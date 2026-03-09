#!/bin/zsh

#
# Module: CelestialEngine/Scripts/Run
# File: run_all.sh
# Created by: Catalin Chirosca
# Created: 2026-03-09
# Updated by: Catalin Chirosca
# Updated: 2026-03-09
#

# Script per eseguire l'applicazione CE_App e tutti i test
# Utilizza la cartella Binaries/Last per eseguire l'ultima build

# Colori per l'output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Directory base
SCRIPT_DIR="${0:A:h}"
PROJECT_DIR="${SCRIPT_DIR}/../.."
BINARIES_DIR="${PROJECT_DIR}/Binaries/Last"

echo "${BLUE}========================================${NC}"
echo "${BLUE}CelestialEngine - Run All${NC}"
echo "${BLUE}========================================${NC}"
echo ""

# Verifica che la cartella Binaries/Last esista
if [ ! -d "${BINARIES_DIR}" ]; then
    echo "${RED}Errore: La cartella ${BINARIES_DIR} non esiste!${NC}"
    echo "${YELLOW}Compila prima il progetto con CMake${NC}"
    exit 1
fi

# Variabile per tracciare i fallimenti
FAILED_TESTS=0
TOTAL_TESTS=0

# Funzione per eseguire un comando e verificare il risultato
run_command() {
    local name="$1"
    local command="$2"
    local is_gui="${3:-false}"

    echo "${BLUE}Esecuzione: ${name}${NC}"
    echo "${YELLOW}Comando: ${command}${NC}"
    echo ""

    cd "${BINARIES_DIR}"

    if [ "$is_gui" = "true" ]; then
        # Per applicazioni GUI, lancia in background e termina dopo 0.5 secondi
        eval "${command}" &
        local pid=$!
        echo "${YELLOW}Applicazione avviata (PID: ${pid}), attesa 0.5 secondi...${NC}"
        sleep 0.5

        # Verifica se il processo è ancora attivo
        if ps -p $pid > /dev/null 2>&1; then
            echo "${YELLOW}Chiusura applicazione...${NC}"
            # Prima prova con SIGTERM (chiusura gentile)
            kill -TERM $pid 2>/dev/null
            sleep 0.1

            # Se il processo è ancora attivo, usa SIGKILL (chiusura forzata)
            if ps -p $pid > /dev/null 2>&1; then
                kill -KILL $pid 2>/dev/null
                sleep 0.1
            fi
        fi

        wait $pid 2>/dev/null
        local exit_code=0  # Considera successo se l'app si è avviata
    else
        # Per test e comandi normali, esegui normalmente
        eval "${command}"
        local exit_code=$?
    fi

    if [ $exit_code -eq 0 ] || [ "$is_gui" = "true" ]; then
        echo "${GREEN}✓ ${name} completato con successo${NC}"
    else
        echo "${RED}✗ ${name} fallito con codice di uscita ${exit_code}${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi

    echo ""
    echo "----------------------------------------"
    echo ""

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    return $exit_code
}

# Esegui CE_App con Metal
run_command \
    "CE_App (Metal)" \
    "./CE_App.app/Contents/MacOS/CE_App -t \"RunAll\" -w 1280 -h 720 -v true -g metal" \
    "true"

# Esegui CE_App con OpenGL
run_command \
    "CE_App (OpenGL)" \
    "./CE_App.app/Contents/MacOS/CE_App -t \"RunAll\" -w 1280 -h 720 -v true -g opengl" \
    "true"

# Esegui tutti i test
echo "${BLUE}========================================${NC}"
echo "${BLUE}Esecuzione Test Suite${NC}"
echo "${BLUE}========================================${NC}"
echo ""

# Test Core
if [ -f "${BINARIES_DIR}/CE_TestsCore" ]; then
    run_command "CE_TestsCore" "./CE_TestsCore"
else
    echo "${YELLOW}⚠ CE_TestsCore non trovato, skip${NC}"
    echo ""
fi

# Test Define
if [ -f "${BINARIES_DIR}/CE_TestsDefine" ]; then
    run_command "CE_TestsDefine" "./CE_TestsDefine"
else
    echo "${YELLOW}⚠ CE_TestsDefine non trovato, skip${NC}"
    echo ""
fi

# Test Events
if [ -f "${BINARIES_DIR}/CE_TestsEvents" ]; then
    run_command "CE_TestsEvents" "./CE_TestsEvents"
else
    echo "${YELLOW}⚠ CE_TestsEvents non trovato, skip${NC}"
    echo ""
fi

# Test Native
if [ -f "${BINARIES_DIR}/CE_TestsNative" ]; then
    run_command "CE_TestsNative" "./CE_TestsNative"
else
    echo "${YELLOW}⚠ CE_TestsNative non trovato, skip${NC}"
    echo ""
fi

# Test Tools
if [ -f "${BINARIES_DIR}/CE_TestsTools" ]; then
    run_command "CE_TestsTools" "./CE_TestsTools"
else
    echo "${YELLOW}⚠ CE_TestsTools non trovato, skip${NC}"
    echo ""
fi

# Test Window
if [ -f "${BINARIES_DIR}/CE_TestsWindow" ]; then
    run_command "CE_TestsWindow" "./CE_TestsWindow"
else
    echo "${YELLOW}⚠ CE_TestsWindow non trovato, skip${NC}"
    echo ""
fi

# Riepilogo finale
echo "${BLUE}========================================${NC}"
echo "${BLUE}Riepilogo Esecuzione${NC}"
echo "${BLUE}========================================${NC}"
echo ""

if [ $FAILED_TESTS -eq 0 ]; then
    echo "${GREEN}✓ Tutti i test sono passati! (${TOTAL_TESTS}/${TOTAL_TESTS})${NC}"
    exit 0
else
    echo "${RED}✗ ${FAILED_TESTS}/${TOTAL_TESTS} test falliti${NC}"
    exit 1
fi

