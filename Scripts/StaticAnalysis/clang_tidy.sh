#!/bin/zsh

#
# Module: CelestialEngine/Scripts/StaticAnalysis
# File: clang_tidy.sh
# Created by: Catalin Chirosca
# Created: 2026-09-18
# Updated by: Catalin Chirosca
# Updated: 2026-09-18
#

# Runs clang-tidy over the project's own sources, with the checks listed in the root .clang-tidy.
#
# The clang-analyzer-* family is part of that list, so this is also the Clang Static Analyzer run: path-sensitive
# checks for null dereferences, leaks, use-after-move and the like. Headers are reported only when they belong to the
# project (Engine/Modules, App, Tests); vendored code and vcpkg dependencies are compiled but never reported.

source "${0:A:h}/common.sh"

SCRIPT_NAME="${0}"

TOOLCHAIN="clang"
CONFIG="Debug"
JOBS="$(sysctl -n hw.ncpu 2>/dev/null || nproc)"
FIX=false
CHECKS=""
FILTER=""
QUIET=false

print_usage() {
	echo "Usage: ${SCRIPT_NAME} [OPTIONS] [FILE_REGEX]"
	echo ""
	echo "Runs clang-tidy (including the Clang Static Analyzer checks) on the project's sources."
	echo "The check list lives in .clang-tidy at the project root."
	echo ""
	echo "Options:"
	echo "  -t, --toolchain <name>   Build directory to take the compile commands from (default: clang). Known: ${ANALYSIS_TOOLCHAINS[*]}"
	echo "  -c, --config <name>      Configuration whose flags are used (default: Debug). Known: ${ALL_CONFIGS[*]}"
	echo "  -j, --jobs <n>           Parallel clang-tidy processes (default: ${JOBS})"
	echo "      --checks <list>      Override the checks, e.g. '-*,bugprone-*' (appended to .clang-tidy)"
	echo "      --fix                Apply the fixes clang-tidy can make on its own. Review the diff afterwards!"
	echo "  -q, --quiet              Print only the summary, the full log is still written to the report"
	echo "  -h, --help               Show this help message"
	echo ""
	echo "FILE_REGEX restricts the analysis to the translation units whose path matches it."
	echo ""
	echo "Examples:"
	echo "  ${SCRIPT_NAME}                                   # everything"
	echo "  ${SCRIPT_NAME} Render/                           # only the renderer sources"
	echo "  ${SCRIPT_NAME} --checks '-*,performance-*'       # a single family"
}

while [[ $# -gt 0 ]]; do
	case $1 in
		-t|--toolchain) TOOLCHAIN="$2"; shift 2 ;;
		-c|--config) CONFIG="$2"; shift 2 ;;
		-j|--jobs) JOBS="$2"; shift 2 ;;
		--checks) CHECKS="$2"; shift 2 ;;
		--fix) FIX=true; shift ;;
		-q|--quiet) QUIET=true; shift ;;
		-h|--help) print_usage; exit 0 ;;
		-*)
			echo "${RED}Unknown option: $1${NC}"
			echo ""
			print_usage
			exit 1
			;;
		*) FILTER="$1"; shift ;;
	esac
done

ce_validate_selection "${TOOLCHAIN}" "${CONFIG}" || exit 1

CLANG_TIDY="$(ce_find_llvm_tool clang-tidy)" || {
	echo "${RED}Error: clang-tidy not found. Install it with 'brew install llvm'.${NC}"
	exit 1
}
RUN_CLANG_TIDY="$(ce_find_llvm_tool run-clang-tidy)" || {
	echo "${RED}Error: run-clang-tidy not found next to clang-tidy. Install it with 'brew install llvm'.${NC}"
	exit 1
}
# Only needed by --fix, which collects the fixes of every translation unit and applies them once, de-duplicated.
APPLY_REPLACEMENTS="$(ce_find_llvm_tool clang-apply-replacements)"

REPORT_DIR="${REPORTS_ROOT}/clang-tidy/${TOOLCHAIN}-${CONFIG}"
REPORT="${REPORT_DIR}/report.txt"

echo "${BLUE}========================================${NC}"
echo "${BLUE}CelestialEngine - clang-tidy${NC}"
echo "${BLUE}========================================${NC}"
echo "clang-tidy:    ${CLANG_TIDY} ($("${CLANG_TIDY}" --version | grep -m1 -o 'version [0-9.]*'))"
echo "Commands from: .build/${TOOLCHAIN} (${CONFIG})"
[ -n "${FILTER}" ] && echo "Files:         ${FILTER}"
[ -n "${CHECKS}" ] && echo "Checks:        ${CHECKS}"
[ "${FIX}" = true ] && echo "${YELLOW}Mode:          FIX (sources will be modified)${NC}"
echo ""

echo -n "Compile commands: "
ce_make_compdb "${TOOLCHAIN}" "${CONFIG}" "${REPORT_DIR}" || exit 1

# Only the project's own headers are worth a diagnostic; the regex is matched against absolute paths.
HEADER_FILTER="${PROJECT_DIR}/(Engine/Modules|App|Tests)/"

TIDY_ARGS=(
	-clang-tidy-binary "${CLANG_TIDY}"
	-p "${REPORT_DIR}"
	-j "${JOBS}"
	-header-filter "${HEADER_FILTER}"
	-quiet
	# The project builds with -Werror; a compiler warning must not turn into a clang-tidy error and hide the checks.
	-extra-arg=-Wno-error
)
# Joined with "=": a value starting with "-*" would otherwise be parsed as an option of its own.
[ -n "${CHECKS}" ] && TIDY_ARGS+=("-checks=${CHECKS}")
if [ "${FIX}" = true ]; then
	if [ -z "${APPLY_REPLACEMENTS}" ]; then
		echo "${RED}Error: --fix needs clang-apply-replacements. Install it with 'brew install llvm'.${NC}"
		exit 1
	fi
	TIDY_ARGS+=(-fix -clang-apply-replacements-binary "${APPLY_REPLACEMENTS}")
fi
[ -n "${FILTER}" ] && TIDY_ARGS+=("${FILTER}")

cd "${PROJECT_DIR}" || exit 1

START=$(date +%s)
if [ "${QUIET}" = true ]; then
	"${RUN_CLANG_TIDY}" "${TIDY_ARGS[@]}" > "${REPORT}" 2>&1
	RC=$?
else
	"${RUN_CLANG_TIDY}" "${TIDY_ARGS[@]}" 2>&1 | tee "${REPORT}"
	RC=${pipestatus[1]}
fi
ELAPSED=$(( $(date +%s) - START ))

# run-clang-tidy exits non-zero for any finding too, so only a run that produced no diagnostic at all is a failure of
# the tool itself (bad arguments, missing compile commands...).
if [ ${RC} -ne 0 ] && ! grep -qE ': (warning|error): ' "${REPORT}"; then
	tail -5 "${REPORT}"
	echo "${RED}Error: run-clang-tidy failed (exit code ${RC}), see ${REPORT}${NC}"
	exit ${RC}
fi

# The same header diagnostic is printed once per translation unit that includes it, so count unique locations. The
# path-sensitive clang-analyzer checks ignore -header-filter and can end a path inside vendored code: only the
# project's own files are counted.
DIAGNOSTICS="$(grep -E "^${PROJECT_DIR}/(Engine/Modules|App|Tests)/.+:[0-9]+:[0-9]+: (warning|error): " "${REPORT}" | sort -u)"
WARNINGS=$(echo -n "${DIAGNOSTICS}" | grep -c ': warning: ')
ERRORS=$(echo -n "${DIAGNOSTICS}" | grep -c ': error: ')

echo ""
echo "${BLUE}========================================${NC}"
echo "${BLUE}Summary (${ELAPSED}s)${NC}"
echo "${BLUE}========================================${NC}"
if [ -n "${DIAGNOSTICS}" ]; then
	echo "By check:"
	echo "${DIAGNOSTICS}" | grep -oE '\[[a-z0-9.,-]+\]$' | sort | uniq -c | sort -rn | sed 's/^/  /'
	echo ""
fi
echo "Report: ${REPORT}"

if [ "${ERRORS}" -gt 0 ]; then
	echo "${RED}${ERRORS} error(s), ${WARNINGS} warning(s)${NC}"
	exit 1
elif [ "${WARNINGS}" -gt 0 ]; then
	echo "${YELLOW}${WARNINGS} warning(s)${NC}"
	exit 1
fi
echo "${GREEN}No issues found${NC}"
