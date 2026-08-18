#!/bin/zsh

#
# Module: CelestialEngine/Scripts/Run
# File: run_all.sh
# Created by: Catalin Chirosca
# Created: 2026-03-09
# Updated by: Catalin Chirosca
# Updated: 2026-04-18
#

# Script to run the CelestialEngine application and all tests
# Uses the Binaries/Last folder to run the latest build

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Base directories
SCRIPT_DIR="${0:A:h}"
PROJECT_DIR="${SCRIPT_DIR}/../.."
BINARIES_DIR="${PROJECT_DIR}/Binaries/Last"

# Parse command line arguments
RUN_GUI_APPS=true
RUN_TESTS=true

while [[ $# -gt 0 ]]; do
	case $1 in
		--no-gui)
			RUN_GUI_APPS=false
			shift
			;;
		--no-tests)
			RUN_TESTS=false
			shift
			;;
		--tests-only)
			RUN_GUI_APPS=false
			RUN_TESTS=true
			shift
			;;
		--gui-only)
			RUN_GUI_APPS=true
			RUN_TESTS=false
			shift
			;;
		-h|--help)
			echo "Usage: $0 [OPTIONS]"
			echo ""
			echo "Options:"
			echo "  --no-gui       Skip GUI application tests"
			echo "  --no-tests     Skip unit tests"
			echo "  --tests-only   Run only unit tests (no GUI apps)"
			echo "  --gui-only     Run only GUI apps (no unit tests)"
			echo "  -h, --help     Show this help message"
			exit 0
			;;
		*)
			echo "${RED}Unknown option: $1${NC}"
			exit 1
			;;
	esac
done

echo "${BLUE}========================================${NC}"
echo "${BLUE}CelestialEngine - Run All${NC}"
echo "${BLUE}========================================${NC}"
if [ "$RUN_GUI_APPS" = false ]; then
	echo "${YELLOW}GUI apps: SKIPPED${NC}"
fi
if [ "$RUN_TESTS" = false ]; then
	echo "${YELLOW}Unit tests: SKIPPED${NC}"
fi
echo ""

# Verify that the Binaries/Last folder exists
if [ ! -d "${BINARIES_DIR}" ]; then
	echo "${RED}Error: The folder ${BINARIES_DIR} does not exist!${NC}"
	echo "${YELLOW}Please compile the project with CMake first, configuring with${NC}"
	echo "${YELLOW}-DCE_COPY_LAST=ON: nothing is copied to Binaries/Last without it${NC}"
	exit 1
fi

# Variable to track failures
FAILED_TESTS=0
TOTAL_TESTS=0
FAILED_TESTS_FILE=$(mktemp)

# Function to execute a command and verify the result
run_command() {
	local name="$1"
	local command="$2"
	local is_gui="${3:-false}"

	echo "${BLUE}Executing: ${name}${NC}"
	echo "${YELLOW}Command: ${command}${NC}"
	echo ""

	cd "${BINARIES_DIR}" || exit

	# Create temporary file to capture output
	# shellcheck disable=SC2155
	local temp_output=$(mktemp)

	if [ "$is_gui" = "true" ]; then
		# For GUI applications, launch in background and terminate after 0.5 seconds
		eval "${command}" > "$temp_output" 2>&1 &
		local pid=$!
		echo "${YELLOW}Application started (PID: ${pid}), waiting 0.5 seconds...${NC}"
		sleep 0.5

		# Check if the process is still active
		if ps -p $pid > /dev/null 2>&1; then
			echo "${YELLOW}Closing application...${NC}"
			# First try with SIGTERM (graceful shutdown)
			kill -TERM $pid 2>/dev/null
			sleep 0.2

			# If the process is still active, use SIGKILL (forced shutdown)
			if ps -p $pid > /dev/null 2>&1; then
				kill -KILL $pid 2>/dev/null
				sleep 0.2
			fi
		fi

		wait $pid 2>/dev/null
		local exit_code=0  # Consider success if the app started

		# Wait additional time to ensure window system cleanup
		# This prevents conflicts with subsequent tests
		sleep 0.3

		# Display output
		cat "$temp_output"
	else
		# For tests and normal commands, execute and capture output
		# Use a subshell to capture both output and exit code
		(eval "${command}") > "$temp_output" 2>&1
		local exit_code=$?
		# Display the captured output
		cat "$temp_output"
	fi

	if [ "$exit_code" -eq 0 ] || [ "$is_gui" = "true" ]; then
		echo "${GREEN}✓ ${name} completed successfully${NC}"
	else
		echo "${RED}✗ ${name} failed with exit code ${exit_code}${NC}"
		FAILED_TESTS=$((FAILED_TESTS + 1))

		# Write failure details to file
		# shellcheck disable=SC2129
		echo "=======================================" >> "$FAILED_TESTS_FILE"
		echo "Test: ${name}" >> "$FAILED_TESTS_FILE"
		echo "Exit Code: ${exit_code}" >> "$FAILED_TESTS_FILE"
		echo "Error details:" >> "$FAILED_TESTS_FILE"
		echo "---------------------------------------" >> "$FAILED_TESTS_FILE"

		# For crashes (exit codes > 128), capture full output
		# For normal test failures, extract failure information
		if [ "$exit_code" -gt 128 ]; then
			# shellcheck disable=SC2129
			echo "CRASH DETECTED (Signal $((exit_code - 128)))" >> "$FAILED_TESTS_FILE"
			echo "Full output:" >> "$FAILED_TESTS_FILE"
			tail -100 "$temp_output" >> "$FAILED_TESTS_FILE" 2>/dev/null
		else
			grep -E "\[  FAILED  \]|Failure|Error|Expected:" "$temp_output" | head -30 >> "$FAILED_TESTS_FILE" 2>/dev/null || echo "(No specific error details captured)" >> "$FAILED_TESTS_FILE"
		fi
		echo "" >> "$FAILED_TESTS_FILE"
	fi

	# Clean up temporary file
	rm -f "$temp_output"

	echo ""
	echo "----------------------------------------"
	echo ""

	TOTAL_TESTS=$((TOTAL_TESTS + 1))
	return $exit_code
}

# Execute GUI applications if requested
if [ "$RUN_GUI_APPS" = true ]; then
	echo "${BLUE}========================================${NC}"
	echo "${BLUE}Executing GUI Applications${NC}"
	echo "${BLUE}========================================${NC}"
	echo ""

	# Execute CelestialEngine with OpenGL
	run_command \
		"CelestialEngine (OpenGLGlfw)" \
		"./CelestialEngine.app/Contents/MacOS/CelestialEngine -t \"RunAll OpenGlGlfw\" -w 1280 -h 720 -v true -g opengl -wa glfw" \
		"true"

	run_command \
		"CelestialEngine (MetalCocoa)" \
		"./CelestialEngine.app/Contents/MacOS/CelestialEngine -t \"RunAll MetalCocoa\" -w 1280 -h 720 -v true -g metal -wa cocoa" \
		"true"

	# Wait for window system to stabilize after GUI tests
	echo "${YELLOW}Waiting for window system cleanup...${NC}"
	sleep 1

	# Ensure all CelestialEngine processes are terminated
	echo "${YELLOW}Checking for remaining CelestialEngine processes...${NC}"
	if pgrep -f "CelestialEngine.app" > /dev/null 2>&1; then
		echo "${YELLOW}Found running CelestialEngine processes, terminating...${NC}"
		pkill -TERM -f "CelestialEngine.app" 2>/dev/null
		sleep 0.5
		# Force kill if still running
		if pgrep -f "CelestialEngine.app" > /dev/null 2>&1; then
			echo "${YELLOW}Force killing remaining CelestialEngine processes...${NC}"
			pkill -KILL -f "CelestialEngine.app" 2>/dev/null
			sleep 0.5
		fi
		echo "${GREEN}✓ All CelestialEngine processes terminated${NC}"
	else
		echo "${GREEN}✓ No CelestialEngine processes running${NC}"
	fi
	echo ""
fi

# Execute all tests
if [ "$RUN_TESTS" = true ]; then

# Test Core
if [ -f "${BINARIES_DIR}/CE_TestsCore" ]; then
	run_command "CE_TestsCore" "./CE_TestsCore"
else
	echo "${YELLOW}⚠ CE_TestsCore not found, skipping${NC}"
	echo ""
fi

# Test Define
if [ -f "${BINARIES_DIR}/CE_TestsDefine" ]; then
	run_command "CE_TestsDefine" "./CE_TestsDefine"
else
	echo "${YELLOW}⚠ CE_TestsDefine not found, skipping${NC}"
	echo ""
fi

# Test Events
if [ -f "${BINARIES_DIR}/CE_TestsEvents" ]; then
	run_command "CE_TestsEvents" "./CE_TestsEvents"
else
	echo "${YELLOW}⚠ CE_TestsEvents not found, skipping${NC}"
	echo ""
fi

# Test Native
if [ -f "${BINARIES_DIR}/CE_TestsNative" ]; then
	run_command "CE_TestsNative" "./CE_TestsNative"
else
	echo "${YELLOW}⚠ CE_TestsNative not found, skipping${NC}"
	echo ""
fi

# Test Tools
if [ -f "${BINARIES_DIR}/CE_TestsTools" ]; then
	run_command "CE_TestsTools" "./CE_TestsTools"
else
	echo "${YELLOW}⚠ CE_TestsTools not found, skipping${NC}"
	echo ""
fi

# Test Window
if [ -f "${BINARIES_DIR}/CE_TestsWindow" ]; then
	run_command "CE_TestsWindow" "./CE_TestsWindow"
else
	echo "${YELLOW}⚠ CE_TestsWindow not found, skipping${NC}"
	echo ""
fi

fi  # End of RUN_TESTS block

# Final summary
echo "${BLUE}========================================${NC}"
echo "${BLUE}Execution Summary${NC}"
echo "${BLUE}========================================${NC}"
echo ""

if [ $FAILED_TESTS -eq 0 ]; then
	echo "${GREEN}✓ All tests passed! (${TOTAL_TESTS}/${TOTAL_TESTS})${NC}"
	rm -f "$FAILED_TESTS_FILE"
	exit 0
else
	echo "${RED}✗ ${FAILED_TESTS}/${TOTAL_TESTS} tests failed${NC}"
	echo ""

	# Display details of failed tests
	if [ -s "$FAILED_TESTS_FILE" ]; then
		echo "${RED}========================================${NC}"
		echo "${RED}Failed Tests Details${NC}"
		echo "${RED}========================================${NC}"
		echo ""

		cat "$FAILED_TESTS_FILE"
	fi

	rm -f "$FAILED_TESTS_FILE"
	exit 1
fi

