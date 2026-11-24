#!/bin/zsh

#
# Module: CelestialEngine/Scripts/Maintenance
# File: bump_updated_dates.sh
# Created by: Catalin Chirosca
# Created: 2026-11-24
# Updated by: Catalin Chirosca
# Updated: 2026-11-24
#

# Sets the 'Updated:' line of the file banner to today's date (YYYY-MM-DD) in every file git sees as changed.
#
# The banner rule (CLAUDE.md, "File Banner") wants that date bumped on every modification. This applies it after the
# fact: by default to every file that differs from HEAD (staged or not) plus the untracked ones not ignored, or only to
# the staged ones with --staged, or to the files given on the command line. Both comment styles are handled:
#
#   // Updated: 2026-09-02      (C++, Objective-C++, Metal, GLSL)
#   #  Updated: 2026-09-02      (shell, CMake)
#
# Only the banner is touched: the first such line within the first BANNER_LINES lines of the file, and only when it
# already holds a YYYY-MM-DD date. 'Created:' and 'Updated by:' are never changed. Deleted and binary files are skipped.

SCRIPT_NAME="${0}"
PROJECT_DIR="${0:A:h:h:h}"

RED=$'\033[0;31m'
GREEN=$'\033[0;32m'
YELLOW=$'\033[0;33m'
BLUE=$'\033[0;34m'
NC=$'\033[0m'

# The banner is the first thing in a file (after a shebang): 15 lines leave room for that and for a blank line or two.
BANNER_LINES=15

MODE="changed"
DRY_RUN=false
TODAY="$(date +%Y-%m-%d)"
FILES=()

print_usage() {
	echo "Usage: ${SCRIPT_NAME} [OPTIONS] [FILE...]"
	echo ""
	echo "Sets the banner's 'Updated:' date ('// Updated:' or '# Updated:') to today in the changed files."
	echo ""
	echo "Options:"
	echo "  -s, --staged         Only the files staged in the index (e.g. before a commit)"
	echo "  -d, --date <date>    Date to write instead of today (YYYY-MM-DD)"
	echo "  -n, --dry-run        List what would change, write nothing"
	echo "  -h, --help           Show this help message"
	echo ""
	echo "Without FILE arguments the files are taken from git: everything that differs from HEAD (staged or not)"
	echo "plus the untracked files that are not ignored."
	echo ""
	echo "Examples:"
	echo "  ${SCRIPT_NAME}                          # every changed file"
	echo "  ${SCRIPT_NAME} --staged                 # only what is about to be committed"
	echo "  ${SCRIPT_NAME} -n                       # preview"
	echo "  ${SCRIPT_NAME} Engine/src/source/EntryPoint.cpp"
}

while [[ $# -gt 0 ]]; do
	case $1 in
		-s|--staged) MODE="staged"; shift ;;
		-d|--date) TODAY="$2"; shift 2 ;;
		-n|--dry-run) DRY_RUN=true; shift ;;
		-h|--help) print_usage; exit 0 ;;
		--) shift; FILES+=("$@"); break ;;
		-*)
			echo "${RED}Unknown option: $1${NC}"
			echo ""
			print_usage
			exit 1
			;;
		*) FILES+=("$1"); shift ;;
	esac
done

if [[ ! "${TODAY}" =~ '^[0-9]{4}-[0-9]{2}-[0-9]{2}$' ]]; then
	echo "${RED}Error: '${TODAY}' is not a YYYY-MM-DD date.${NC}"
	exit 1
fi

cd "${PROJECT_DIR}" || exit 1

if [ ${#FILES[@]} -eq 0 ]; then
	git rev-parse --is-inside-work-tree >/dev/null 2>&1 || {
		echo "${RED}Error: ${PROJECT_DIR} is not a git work tree.${NC}"
		exit 1
	}

	# -z and NUL splitting: paths with spaces stay whole. --diff-filter=d drops deletions, there is nothing to edit.
	if [ "${MODE}" = "staged" ]; then
		FILES=("${(@0)$(git diff --cached --name-only -z --diff-filter=d)}")
	else
		FILES=(
			"${(@0)$(git diff HEAD --name-only -z --diff-filter=d)}"
			"${(@0)$(git ls-files --others --exclude-standard -z)}"
		)
	fi

	# Drop the empty elements the splits leave behind and any duplicate.
	FILES=("${(@u)FILES:#}")
fi

echo "${BLUE}========================================${NC}"
echo "${BLUE}CelestialEngine - bump 'Updated:' dates${NC}"
echo "${BLUE}========================================${NC}"
echo "Date:  ${TODAY}"
echo "Files: ${#FILES[@]} candidate(s)$([ "${DRY_RUN}" = true ] && echo ' (dry run)')"
echo ""

UPDATED=0
CURRENT=0
NO_BANNER=0

for file in "${FILES[@]}"; do
	# Deleted in the work tree but still in the index, a directory, or binary: nothing to do.
	[ -f "${file}" ] || continue
	grep -Iq . "${file}" 2>/dev/null || continue

	# Line number and date of the banner's 'Updated:' line: '//' or '#', optional spaces, then a YYYY-MM-DD date.
	match="$(head -n "${BANNER_LINES}" "${file}" \
		| grep -nE -m 1 '^[[:space:]]*(//|#)[[:space:]]*Updated:[[:space:]]*[0-9]{4}-[0-9]{2}-[0-9]{2}')"

	if [ -z "${match}" ]; then
		NO_BANNER=$((NO_BANNER + 1))
		continue
	fi

	line="${match%%:*}"
	old="$(echo "${match}" | grep -oE '[0-9]{4}-[0-9]{2}-[0-9]{2}' | head -n 1)"

	if [ "${old}" = "${TODAY}" ]; then
		CURRENT=$((CURRENT + 1))
		continue
	fi

	if [ "${DRY_RUN}" = false ]; then
		# Only that one line, and only its date: the comment marker and the spacing stay as they are.
		perl -i -pe "s/(Updated:\\s*)\\d{4}-\\d{2}-\\d{2}/\${1}${TODAY}/ if \$. == ${line}" "${file}" || {
			echo "${RED}  failed   ${file}${NC}"
			continue
		}
	fi

	echo "${GREEN}  ${old} -> ${TODAY}${NC}  ${file}"
	UPDATED=$((UPDATED + 1))
done

echo ""
echo "${GREEN}Updated: ${UPDATED}${NC}  already ${TODAY}: ${CURRENT}  ${YELLOW}no banner: ${NO_BANNER}${NC}"
