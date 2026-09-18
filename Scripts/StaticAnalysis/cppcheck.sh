#!/bin/zsh

#
# Module: CelestialEngine/Scripts/StaticAnalysis
# File: cppcheck.sh
# Created by: Catalin Chirosca
# Created: 2026-09-18
# Updated by: Catalin Chirosca
# Updated: 2026-09-18
#

# Runs cppcheck over the project's own C++ sources.
#
# cppcheck has its own parser, independent from Clang, so it catches a different set of problems than clang-tidy
# (and misses the ones that need full semantic analysis). The Objective-C++ files (.mm) are left out: cppcheck
# cannot parse them. Findings that are judged false positives go to cppcheck-suppressions.txt next to this script,
# or inline as '// cppcheck-suppress <id>' on the line above.

source "${0:A:h}/common.sh"

SCRIPT_NAME="${0}"

TOOLCHAIN="clang"
CONFIG="Debug"
JOBS="$(sysctl -n hw.ncpu 2>/dev/null || nproc)"
HTML=false
EXHAUSTIVE=false
FILTER=""

print_usage() {
	echo "Usage: ${SCRIPT_NAME} [OPTIONS] [FILE_REGEX]"
	echo ""
	echo "Runs cppcheck on the project's C++ sources (the .mm files are skipped, cppcheck cannot parse them)."
	echo ""
	echo "Options:"
	echo "  -t, --toolchain <name>   Build directory to take the compile commands from (default: clang). Known: ${ANALYSIS_TOOLCHAINS[*]}"
	echo "  -c, --config <name>      Configuration whose defines are used (default: Debug). Known: ${ALL_CONFIGS[*]}"
	echo "  -j, --jobs <n>           Parallel cppcheck jobs (default: ${JOBS})"
	echo "      --exhaustive         Exhaustive value-flow analysis: slower, fewer missed paths"
	echo "      --html               Also write an HTML report (cppcheck-htmlreport)"
	echo "  -h, --help               Show this help message"
	echo ""
	echo "FILE_REGEX restricts the analysis to the translation units whose path matches it."
	echo ""
	echo "Examples:"
	echo "  ${SCRIPT_NAME}                     # everything"
	echo "  ${SCRIPT_NAME} --html Utility/     # only the Utility module, with an HTML report"
}

while [[ $# -gt 0 ]]; do
	case $1 in
		-t|--toolchain) TOOLCHAIN="$2"; shift 2 ;;
		-c|--config) CONFIG="$2"; shift 2 ;;
		-j|--jobs) JOBS="$2"; shift 2 ;;
		--exhaustive) EXHAUSTIVE=true; shift ;;
		--html) HTML=true; shift ;;
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

CPPCHECK="$(command -v cppcheck)" || {
	echo "${RED}Error: cppcheck not found. Install it with 'brew install cppcheck'.${NC}"
	exit 1
}

REPORT_DIR="${REPORTS_ROOT}/cppcheck/${TOOLCHAIN}-${CONFIG}"
REPORT="${REPORT_DIR}/report.txt"
XML_REPORT="${REPORT_DIR}/report.xml"
SUPPRESSIONS="${STATIC_ANALYSIS_DIR}/cppcheck-suppressions.txt"

echo "${BLUE}========================================${NC}"
echo "${BLUE}CelestialEngine - cppcheck${NC}"
echo "${BLUE}========================================${NC}"
echo "cppcheck:      ${CPPCHECK} ($("${CPPCHECK}" --version))"
echo "Commands from: .build/${TOOLCHAIN} (${CONFIG})"
[ -n "${FILTER}" ] && echo "Files:         ${FILTER}"
echo ""

echo -n "Compile commands: "
ce_make_compdb "${TOOLCHAIN}" "${CONFIG}" "${REPORT_DIR}" false || exit 1

if [ -n "${FILTER}" ]; then
	python3 - "${REPORT_DIR}/compile_commands.json" "${FILTER}" <<'EOF' || exit 1
import json, re, sys
path, pattern = sys.argv[1], re.compile(sys.argv[2])
entries = [e for e in json.load(open(path)) if pattern.search(e["file"])]
json.dump(entries, open(path, "w"), indent=1)
print(f"{len(entries)} translation units match the filter")
if not entries:
    sys.exit(1)
EOF
fi

# The build dir caches per-file results, so a second run only re-analyzes what changed.
mkdir -p "${REPORT_DIR}/cache"

CPPCHECK_ARGS=(
	--project="${REPORT_DIR}/compile_commands.json"
	--cppcheck-build-dir="${REPORT_DIR}/cache"
	-j "${JOBS}"
	--enable=warning,style,performance,portability
	--std=c++23
	--library=googletest
	--library=opengl
	--inline-suppr
	--suppressions-list="${SUPPRESSIONS}"
	# Third-party code is parsed for the types it declares but never reported.
	--suppress="*:${PROJECT_DIR}/.build/*"
	--suppress="*:${PROJECT_DIR}/Engine/Submodules/*"
	--suppress="*:${PROJECT_DIR}/vcpkg_link/*"
	--check-level="$([ "${EXHAUSTIVE}" = true ] && echo exhaustive || echo normal)"
	--quiet
	--xml
	--output-file="${XML_REPORT}"
	--error-exitcode=0
)

cd "${PROJECT_DIR}" || exit 1

START=$(date +%s)
"${CPPCHECK}" "${CPPCHECK_ARGS[@]}"
RC=$?
ELAPSED=$(( $(date +%s) - START ))
if [ ${RC} -ne 0 ]; then
	echo "${RED}Error: cppcheck exited with ${RC}${NC}"
	exit ${RC}
fi

# The XML report is the complete one; report.txt is a readable, de-duplicated view of it (a header is analyzed once
# per translation unit that includes it), and by_check.txt the per-check counts shown in the summary.
python3 - "${XML_REPORT}" "${REPORT}" "${REPORT_DIR}/by_check.txt" "${PROJECT_DIR}" <<'EOF' || exit 1
import os, sys, xml.etree.ElementTree as ET
from collections import Counter

xml_path, txt_path, counts_path, root = sys.argv[1:5]
seen, lines, by_id = set(), [], Counter()
for error in ET.parse(xml_path).getroot().iter("error"):
    location = error.find("location")
    where = "(no location)"
    if location is not None:
        where = f'{os.path.relpath(location.get("file"), root)}:{location.get("line")}:{location.get("column")}'
    key = (where, error.get("id"))
    if key in seen:
        continue
    seen.add(key)
    by_id[error.get("id")] += 1
    lines.append(f'{where}: {error.get("severity")}: {error.get("msg")} [{error.get("id")}]')

lines.sort()
open(txt_path, "w").write("".join(line + "\n" for line in lines))
open(counts_path, "w").write("".join(f"{count:5} {check}\n" for check, count in by_id.most_common()))
EOF

cat "${REPORT}"
TOTAL=$(wc -l < "${REPORT}" | tr -d ' ')
ERRORS=$(grep -c ': error: ' "${REPORT}")

if [ "${HTML}" = true ]; then
	if command -v cppcheck-htmlreport >/dev/null; then
		cppcheck-htmlreport --file="${XML_REPORT}" --report-dir="${REPORT_DIR}/html" \
			--source-dir="${PROJECT_DIR}" --title="CelestialEngine" >/dev/null
	else
		echo "${YELLOW}cppcheck-htmlreport not found, HTML report skipped${NC}"
	fi
fi

echo ""
echo "${BLUE}========================================${NC}"
echo "${BLUE}Summary (${ELAPSED}s)${NC}"
echo "${BLUE}========================================${NC}"
if [ "${TOTAL}" -gt 0 ]; then
	echo "By check:"
	sed 's/^/  /' "${REPORT_DIR}/by_check.txt"
	echo ""
fi
echo "Report: ${REPORT}"
[ -f "${REPORT_DIR}/html/index.html" ] && echo "HTML:   ${REPORT_DIR}/html/index.html"

if [ "${ERRORS}" -gt 0 ]; then
	echo "${RED}${TOTAL} issue(s), ${ERRORS} of them errors${NC}"
	exit 1
elif [ "${TOTAL}" -gt 0 ]; then
	echo "${YELLOW}${TOTAL} issue(s)${NC}"
	exit 1
fi
echo "${GREEN}No issues found${NC}"
