#!/bin/zsh

#
# Module: CelestialEngine/Scripts/StaticAnalysis
# File: run_static_analysis.sh
# Created by: Catalin Chirosca
# Created: 2026-09-18
# Updated by: Catalin Chirosca
# Updated: 2026-09-18
#

# Runs every static analyzer in turn (clang-tidy, which includes the Clang Static Analyzer, then cppcheck) and prints
# one summary line per tool. Every option not listed below is forwarded to both scripts.

source "${0:A:h}/common.sh"

SCRIPT_NAME="${0}"

RUN_TIDY=true
RUN_CPPCHECK=true
FORWARDED=()

print_usage() {
	echo "Usage: ${SCRIPT_NAME} [OPTIONS] [FILE_REGEX]"
	echo ""
	echo "Runs clang-tidy and cppcheck on the project's sources."
	echo ""
	echo "Options:"
	echo "      --tidy-only          Only clang-tidy"
	echo "      --cppcheck-only      Only cppcheck"
	echo "  -t, --toolchain <name>   Forwarded: build directory to read the compile commands from (default: clang)"
	echo "  -c, --config <name>      Forwarded: configuration whose flags are used (default: Debug)"
	echo "  -j, --jobs <n>           Forwarded: parallel jobs"
	echo "  -h, --help               Show this help message"
	echo ""
	echo "Each tool has more options of its own: see clang_tidy.sh --help and cppcheck.sh --help."
	echo "Reports are written under .build/static-analysis/<tool>/<toolchain>-<config>/."
}

while [[ $# -gt 0 ]]; do
	case $1 in
		--tidy-only) RUN_CPPCHECK=false; shift ;;
		--cppcheck-only) RUN_TIDY=false; shift ;;
		-t|--toolchain|-c|--config|-j|--jobs) FORWARDED+=("$1" "$2"); shift 2 ;;
		-h|--help) print_usage; exit 0 ;;
		-*)
			echo "${RED}Unknown option: $1${NC}"
			echo ""
			print_usage
			exit 1
			;;
		*) FORWARDED+=("$1"); shift ;;
	esac
done

typeset -A RESULTS
TOOLS=()
FAILED=false

if [ "${RUN_TIDY}" = true ]; then
	TOOLS+=(clang-tidy)
	"${STATIC_ANALYSIS_DIR}/clang_tidy.sh" --quiet "${FORWARDED[@]}"
	RESULTS[clang-tidy]=$?
	echo ""
fi

if [ "${RUN_CPPCHECK}" = true ]; then
	TOOLS+=(cppcheck)
	"${STATIC_ANALYSIS_DIR}/cppcheck.sh" "${FORWARDED[@]}"
	RESULTS[cppcheck]=$?
	echo ""
fi

echo "${BLUE}========================================${NC}"
echo "${BLUE}Static analysis${NC}"
echo "${BLUE}========================================${NC}"
for tool in "${TOOLS[@]}"; do
	if [ "${RESULTS[${tool}]}" -eq 0 ]; then
		printf "  %-12s ${GREEN}clean${NC}\n" "${tool}"
	else
		printf "  %-12s ${YELLOW}issues found${NC}\n" "${tool}"
		FAILED=true
	fi
done
echo "Reports: ${REPORTS_ROOT}"

[ "${FAILED}" = true ] && exit 1
exit 0
