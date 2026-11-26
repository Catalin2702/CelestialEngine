#!/bin/zsh

#
# Module: CelestialEngine/Scripts/Maintenance
# File: bump_updated_dates.sh
# Created by: Catalin Chirosca
# Created: 2026-11-24
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
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
# A NEW file - one HEAD does not have: untracked, or added to the index - gets its 'Created:' date set as well, so a
# file started yesterday and committed today carries today on both lines, as the banner rule wants for a new file.
# Renames are not new: a file moved with 'git mv' (or staged after a plain mv) is a rename to git and keeps its
# 'Created:'. A file moved with plain mv and NOT staged looks like a deletion plus an untracked file, so stage the move
# first, or pass --no-created.
#
# Only the banner is touched: the first such line within the first BANNER_LINES lines of the file, and only when it
# already holds a YYYY-MM-DD date. 'Updated by:' and 'Created by:' are never changed. Deleted and binary files are
# skipped.

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
SET_CREATED=true
TODAY="$(date +%Y-%m-%d)"
FILES=()

print_usage() {
	echo "Usage: ${SCRIPT_NAME} [OPTIONS] [FILE...]"
	echo ""
	echo "Sets the banner's 'Updated:' date ('// Updated:' or '# Updated:') to today in the changed files, and the"
	echo "'Created:' date too in the new ones (not in HEAD: untracked or added; renames are not new)."
	echo ""
	echo "Options:"
	echo "  -s, --staged         Only the files staged in the index (e.g. before a commit)"
	echo "  -d, --date <date>    Date to write instead of today (YYYY-MM-DD)"
	echo "  -n, --dry-run        List what would change, write nothing"
	echo "      --no-created     Never touch 'Created:', even in new files"
	echo "  -h, --help           Show this help message"
	echo ""
	echo "Without FILE arguments the files are taken from git: everything that differs from HEAD (staged or not)"
	echo "plus the untracked files that are not ignored."
	echo ""
	echo "Examples:"
	echo "  ${SCRIPT_NAME}                          # every changed file"
	echo "  ${SCRIPT_NAME} --staged                 # only what is about to be committed"
	echo "  ${SCRIPT_NAME} -n                       # preview"
	echo "  ${SCRIPT_NAME} Engine/Source/Launch/source/EntryPoint.cpp"
}

while [[ $# -gt 0 ]]; do
	case $1 in
		-s|--staged) MODE="staged"; shift ;;
		-d|--date) TODAY="$2"; shift 2 ;;
		-n|--dry-run) DRY_RUN=true; shift ;;
		--no-created) SET_CREATED=false; shift ;;
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

IN_GIT=false
git rev-parse --is-inside-work-tree >/dev/null 2>&1 && IN_GIT=true

if [ ${#FILES[@]} -eq 0 ] && [ "${IN_GIT}" = false ]; then
	echo "${RED}Error: ${PROJECT_DIR} is not a git work tree.${NC}"
	exit 1
fi

# Paths given on the command line, relative to the project like git's: an absolute path inside it loses its prefix.
FILES=("${(@)FILES#${PROJECT_DIR}/}")
FILES=("${(@)FILES#./}")

# The new files, as a set. -M turns a delete + add of the same content into a rename (R), which --diff-filter=A leaves
# out: a moved file keeps its 'Created:'. With --staged only the index counts; otherwise the work tree too, plus the
# untracked files. Without a HEAD (no commit yet) every file is new.
typeset -A IS_NEW
NO_HEAD=false
if [ "${IN_GIT}" = true ] && [ "${SET_CREATED}" = true ]; then
	if git rev-parse --verify -q HEAD >/dev/null; then
		if [ "${MODE}" = "staged" ]; then
			new_files=("${(@0)$(git diff --cached -M --name-only -z --diff-filter=A)}")
		else
			new_files=(
				"${(@0)$(git diff HEAD -M --name-only -z --diff-filter=A)}"
				"${(@0)$(git ls-files --others --exclude-standard -z)}"
			)
		fi
		for new_file in "${(@)new_files:#}"; do
			IS_NEW[${new_file}]=1
		done
	else
		NO_HEAD=true
	fi
fi

if [ ${#FILES[@]} -eq 0 ]; then
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
CREATED=0
CURRENT=0
NO_BANNER=0

# Line number and date of the banner's '<field>:' line in a file: '//' or '#', optional spaces, the field, a colon,
# then a YYYY-MM-DD date - so 'Created by:' and 'Updated by:' never match. Prints "<line> <date>", or nothing.
banner_date() {
	local match
	match="$(head -n "${BANNER_LINES}" "$1" \
		| grep -nE -m 1 "^[[:space:]]*(//|#)[[:space:]]*$2:[[:space:]]*[0-9]{4}-[0-9]{2}-[0-9]{2}")"
	[ -n "${match}" ] && echo "${match%%:*} $(echo "${match}" | grep -oE '[0-9]{4}-[0-9]{2}-[0-9]{2}' | head -n 1)"
}

# Writes TODAY into the date of the banner's '<field>:' line, at the given line number. Only that one line, and only
# its date: the comment marker and the spacing stay as they are.
write_date() {
	perl -i -pe "s/($2:\\s*)\\d{4}-\\d{2}-\\d{2}/\${1}${TODAY}/ if \$. == $3" "$1"
}

for file in "${FILES[@]}"; do
	# Deleted in the work tree but still in the index, a directory, or binary: nothing to do.
	[ -f "${file}" ] || continue
	grep -Iq . "${file}" 2>/dev/null || continue

	updated=($(banner_date "${file}" Updated))
	if [ ${#updated[@]} -eq 0 ]; then
		NO_BANNER=$((NO_BANNER + 1))
		continue
	fi

	# A new file gets 'Created:' too, when its banner has one that is not today yet.
	created=()
	if [ "${SET_CREATED}" = true ] && { [ "${NO_HEAD}" = true ] || [ -n "${IS_NEW[${file}]}" ]; }; then
		created=($(banner_date "${file}" Created))
		[ ${#created[@]} -gt 0 ] && [ "${created[2]}" = "${TODAY}" ] && created=()
	fi

	if [ "${updated[2]}" = "${TODAY}" ] && [ ${#created[@]} -eq 0 ]; then
		CURRENT=$((CURRENT + 1))
		continue
	fi

	changes=()
	if [ "${updated[2]}" != "${TODAY}" ]; then
		if [ "${DRY_RUN}" = false ] && ! write_date "${file}" Updated "${updated[1]}"; then
			echo "${RED}  failed   ${file}${NC}"
			continue
		fi
		changes+=("updated ${updated[2]} -> ${TODAY}")
		UPDATED=$((UPDATED + 1))
	fi
	if [ ${#created[@]} -gt 0 ]; then
		if [ "${DRY_RUN}" = false ] && ! write_date "${file}" Created "${created[1]}"; then
			echo "${RED}  failed   ${file}${NC}"
			continue
		fi
		changes+=("created ${created[2]} -> ${TODAY}")
		CREATED=$((CREATED + 1))
	fi

	echo "${GREEN}  ${(j:, :)changes}${NC}  ${file}"
done

echo ""
echo "${GREEN}Updated: ${UPDATED}  Created: ${CREATED}${NC}  already ${TODAY}: ${CURRENT}  ${YELLOW}no banner: ${NO_BANNER}${NC}"
