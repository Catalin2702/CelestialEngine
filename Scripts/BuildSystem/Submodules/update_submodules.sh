#!/bin/zsh

#
# Module: CelestialEngine/Scripts/BuildSystem/Submodules
# File: update_submodules.sh
# Created by: Catalin Chirosca
# Created: 2026-09-06
# Updated by: Catalin Chirosca
# Updated: 2026-09-06
#

# Brings every git submodule to the revision pinned in Engine/Submodules/submodules.txt.
#
# The pin file holds ONLY "<name>==<ref>"; the path and URL of each submodule come from
# .gitmodules, so the two files can never disagree about where a submodule lives. A submodule that
# is not checked out yet is initialised on the spot, so this doubles as the after-clone step.
#
# Moving a submodule changes the superproject's gitlink, which is a staged change like any other:
# nothing is committed here, and --stage is offered for the "git add" half of it.

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Base directories
SCRIPT_DIR="${0:A:h}"
PROJECT_DIR="${SCRIPT_DIR}/../../.."
PROJECT_DIR="${PROJECT_DIR:A}" # Collapse the ../.. so the printed paths stay readable

PIN_FILE="${PROJECT_DIR}/Engine/Submodules/submodules.txt"

# Captured here because inside a zsh function $0 is the function name, not the script
# shellcheck disable=SC2034
SCRIPT_NAME="${0}"

WANTED=()
LIST_ONLY=false
CHECK_ONLY=false
DRY_RUN=false
FORCE=false
STAGE=false

print_usage() {
	echo "Usage: ${SCRIPT_NAME} [OPTIONS]"
	echo ""
	echo "Checks every submodule out at the revision pinned in Engine/Submodules/submodules.txt."
	echo ""
	echo "Options:"
	echo "  -s, --submodule <name>   Only this submodule (repeatable). Default: all of them"
	echo "  -l, --list               Show pinned vs current revision and exit"
	echo "      --check              Exit non-zero if anything is off its pin (for CI); changes nothing"
	echo "      --dry-run            Print what would happen without touching the working tree"
	echo "      --force              Move a submodule even if its working tree is dirty"
	echo "      --stage              'git add' the moved submodules in the superproject"
	echo "  -h, --help               Show this help message"
	echo ""
	echo "Examples:"
	echo "  ${SCRIPT_NAME}                       # apply every pin"
	echo "  ${SCRIPT_NAME} --list                # what is pinned, and where each submodule actually is"
	echo "  ${SCRIPT_NAME} -s ImGui --stage      # move just ImGui and stage the gitlink"
	echo "  ${SCRIPT_NAME} --check               # fail if the checkout drifted from the pin file"
}

while [[ $# -gt 0 ]]; do
	case $1 in
		-s|--submodule)
			WANTED+=("$2")
			shift 2
			;;
		-l|--list)
			LIST_ONLY=true
			shift
			;;
		--check)
			CHECK_ONLY=true
			shift
			;;
		--dry-run)
			DRY_RUN=true
			shift
			;;
		--force)
			FORCE=true
			shift
			;;
		--stage)
			STAGE=true
			shift
			;;
		-h|--help)
			print_usage
			exit 0
			;;
		*)
			echo "${RED}Unknown option: $1${NC}"
			echo ""
			print_usage
			exit 1
			;;
	esac
done

cd "${PROJECT_DIR}" || exit 1

if [ ! -f "${PIN_FILE}" ]; then
	echo "${RED}Error: no pin file at ${PIN_FILE}${NC}"
	exit 1
fi

# .gitmodules is the authority on path and URL. Read it once into two name-keyed maps rather than
# shelling out to git config per submodule.
typeset -A SUBMODULE_PATHS
typeset -A SUBMODULE_URLS

while IFS= read -r line; do
	key="${line%% *}"
	value="${line#* }"
	case "${key}" in
		submodule.*.path)
			# Keyed by folder name - that is what the pin file names, and it is the last path segment.
			SUBMODULE_PATHS[${value:t}]="${value}"
			;;
		submodule.*.url)
			submodule_path="${key#submodule.}"
			SUBMODULE_URLS[${${submodule_path%.url}:t}]="${value}"
			;;
	esac
done < <(git config -f .gitmodules --list 2>/dev/null | tr '=' ' ')

# Parse the pin file: "<name>==<ref>", '#' comments and blank lines ignored.
NAMES=()
REFS=()

while IFS= read -r line; do
	line="${line%%#*}"
	line="${line//[[:space:]]/}"
	[ -z "${line}" ] && continue

	if [[ "${line}" != *"=="* ]]; then
		echo "${RED}Error: malformed line in $(basename "${PIN_FILE}"): ${line}${NC}"
		echo "${YELLOW}Expected <name>==<ref>${NC}"
		exit 1
	fi

	NAMES+=("${line%%==*}")
	REFS+=("${line#*==}")
done < "${PIN_FILE}"

if [ ${#NAMES[@]} -eq 0 ]; then
	echo "${RED}Error: $(basename "${PIN_FILE}") pins nothing${NC}"
	exit 1
fi

# A submodule present in .gitmodules but absent from the pin file is almost always an oversight:
# it would silently keep whatever revision the gitlink happens to hold.
for name in ${(k)SUBMODULE_URLS}; do
	if [[ ! " ${NAMES[*]} " == *" ${name} "* ]]; then
		echo "${YELLOW}Warning: ${name} is a submodule but is not pinned in $(basename "${PIN_FILE}")${NC}"
	fi
done

SUMMARY=()
MOVED_PATHS=()
STEPS_FAILED=0
DRIFTED=0

# Prints the tag (or short SHA) a checked-out submodule sits on, or "-" when it has no checkout.
# Note: the variable is sub_path, not path - in zsh $path IS $PATH, and assigning it breaks the script.
describe_current() {
	local sub_path="$1"
	[ -d "${sub_path}/.git" ] || [ -f "${sub_path}/.git" ] || { echo "-"; return; }
	git -C "${sub_path}" describe --tags --always --dirty 2>/dev/null || git -C "${sub_path}" rev-parse --short HEAD 2>/dev/null
}

index=1
for name in "${NAMES[@]}"; do
	ref="${REFS[$index]}"
	index=$((index + 1))

	if [ ${#WANTED[@]} -gt 0 ] && [[ ! " ${WANTED[*]} " == *" ${name} "* ]]; then
		continue
	fi

	sub_path="${SUBMODULE_PATHS[${name}]}"
	if [ -z "${sub_path}" ]; then
		echo "${RED}✗ ${name}: pinned to ${ref} but no such submodule in .gitmodules${NC}"
		SUMMARY+=("${RED}✗${NC} ${name} - not in .gitmodules")
		STEPS_FAILED=$((STEPS_FAILED + 1))
		continue
	fi

	current="$(describe_current "${sub_path}")"

	if [ "${LIST_ONLY}" = true ]; then
		# Resolved without fetching, so this reflects what is on disk right now.
		wanted_sha="$(git -C "${sub_path}" rev-parse -q --verify "${ref}^{commit}" 2>/dev/null)"
		head_sha="$(git -C "${sub_path}" rev-parse -q --verify HEAD 2>/dev/null)"

		if [ -z "${wanted_sha}" ]; then
			mark="${YELLOW}?${NC}" # ref unknown locally - a fetch may be all that is missing
		elif [ "${wanted_sha}" = "${head_sha}" ]; then
			mark="${GREEN}=${NC}"
		else
			mark="${YELLOW}≠${NC}"
		fi

		printf "  %b %-14s pinned=%-24s current=%-34s %s\n" \
			"${mark}" "${name}" "${ref}" "${current}" "${head_sha:0:12}"
		continue
	fi

	echo "${BLUE}==> ${name}${NC} (${sub_path})"

	# A missing checkout is the normal state right after a clone, so initialise instead of failing.
	if [ ! -e "${sub_path}/.git" ]; then
		echo "  not checked out yet, initialising"
		if [ "${DRY_RUN}" = false ] && ! git submodule update --init -- "${sub_path}" > /dev/null; then
			echo "${RED}✗ could not initialise ${name}${NC}"
			SUMMARY+=("${RED}✗${NC} ${name} - init failed")
			STEPS_FAILED=$((STEPS_FAILED + 1))
			continue
		fi
	fi

	# Local edits inside a submodule would be silently carried onto another revision, or block the
	# checkout outright. Better to say so than to guess.
	if [ -n "$(git -C "${sub_path}" status --porcelain 2>/dev/null)" ] && [ "${FORCE}" = false ]; then
		echo "${RED}✗ working tree is dirty - commit, stash, or re-run with --force${NC}"
		SUMMARY+=("${RED}✗${NC} ${name} - dirty working tree")
		STEPS_FAILED=$((STEPS_FAILED + 1))
		continue
	fi

	# --prune-tags matters: a plain "fetch --tags" only ever ADDS tags, so a tag deleted or renamed
	# upstream (27.0.3 -> v27.0.3) survives locally forever and then wins the git-describe lookup,
	# making a correctly checked-out submodule report the old name.
	if [ "${DRY_RUN}" = false ] && ! git -C "${sub_path}" fetch --tags --prune --prune-tags --quiet origin 2>/dev/null; then
		echo "${YELLOW}  could not reach origin, resolving ${ref} against what is already local${NC}"
	fi

	# A branch name means "the remote tip", never a stale local branch of the same name; tags and
	# SHAs have no origin/ counterpart and fall through to the plain ref.
	target="${ref}"
	if git -C "${sub_path}" rev-parse -q --verify "refs/remotes/origin/${ref}^{commit}" > /dev/null 2>&1; then
		target="origin/${ref}"
	fi

	sha="$(git -C "${sub_path}" rev-parse -q --verify "${target}^{commit}" 2>/dev/null)"
	if [ -z "${sha}" ]; then
		echo "${RED}✗ ${ref} does not resolve to a commit in $(basename "${sub_path}")${NC}"
		SUMMARY+=("${RED}✗${NC} ${name} - unknown ref ${ref}")
		STEPS_FAILED=$((STEPS_FAILED + 1))
		continue
	fi

	if [ "$(git -C "${sub_path}" rev-parse HEAD 2>/dev/null)" = "${sha}" ]; then
		echo "${GREEN}  already at ${ref} (${sha:0:12})${NC}"
		SUMMARY+=("${GREEN}=${NC} ${name} ${ref} (unchanged)")
		continue
	fi

	DRIFTED=$((DRIFTED + 1))

	if [ "${CHECK_ONLY}" = true ]; then
		echo "${YELLOW}  drifted: ${current} != ${ref}${NC}"
		SUMMARY+=("${YELLOW}≠${NC} ${name} ${current} -> ${ref}")
		continue
	fi

	if [ "${DRY_RUN}" = true ]; then
		echo "  would check out ${ref} (${sha:0:12}), currently ${current}"
		SUMMARY+=("${YELLOW}~${NC} ${name} ${current} -> ${ref} (dry run)")
		continue
	fi

	# Detached on purpose: the pin file is the only thing that decides the revision, so a submodule
	# should never sit on a local branch that could quietly move underneath it.
	if ! git -C "${sub_path}" checkout --detach --quiet "${sha}"; then
		echo "${RED}✗ could not check out ${ref}${NC}"
		SUMMARY+=("${RED}✗${NC} ${name} - checkout failed")
		STEPS_FAILED=$((STEPS_FAILED + 1))
		continue
	fi

	# Nested submodules of the submodule, if it has any.
	git -C "${sub_path}" submodule update --init --recursive --quiet 2>/dev/null

	echo "${GREEN}  ${current} -> $(describe_current "${sub_path}")${NC}"
	SUMMARY+=("${GREEN}✓${NC} ${name} ${current} -> ${ref}")
	MOVED_PATHS+=("${sub_path}")
done

if [ "${LIST_ONLY}" = true ]; then
	exit 0
fi

if [ ${#MOVED_PATHS[@]} -gt 0 ]; then
	if [ "${STAGE}" = true ]; then
		git add -- "${MOVED_PATHS[@]}"
		echo ""
		echo "${BLUE}Staged the moved gitlinks in the superproject${NC}"
	else
		echo ""
		echo "${YELLOW}The superproject still points at the old revisions. Record the move with:${NC}"
		echo "  git add ${MOVED_PATHS[*]}"
	fi
fi

echo ""
echo "${BLUE}========================================${NC}"
echo "${BLUE}Submodule Summary${NC}"
echo "${BLUE}========================================${NC}"
echo ""

for line in "${SUMMARY[@]}"; do
	echo "  ${line}"
done
echo ""

if [ "${CHECK_ONLY}" = true ]; then
	if [ ${DRIFTED} -eq 0 ] && [ ${STEPS_FAILED} -eq 0 ]; then
		echo "${GREEN}✓ Every submodule is on its pin${NC}"
		exit 0
	fi
	echo "${RED}✗ ${DRIFTED} submodule(s) off their pin${NC}"
	exit 1
fi

if [ ${STEPS_FAILED} -eq 0 ]; then
	if [ "${DRY_RUN}" = true ]; then
		echo "${YELLOW}Dry run: ${DRIFTED} submodule(s) would move, nothing was touched${NC}"
	else
		echo "${GREEN}✓ All submodules are at their pinned revision${NC}"
	fi
	exit 0
fi

echo "${RED}✗ ${STEPS_FAILED} submodule(s) failed${NC}"
exit 1
