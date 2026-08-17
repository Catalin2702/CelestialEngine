#!/bin/zsh

#
# Module: CelestialEngine/Scripts/BuildSystem/All
# File: build_all.sh
# Created by: Catalin Chirosca
# Created: 2026-08-17
# Updated by: Catalin Chirosca
# Updated: 2026-08-17
#

# Script to build (and optionally test) every toolchain x configuration combination.
#
# The build presets in CMakePresets.json deliberately carry no "configuration" field:
# every generator is multi-config, so the configuration is chosen at build time (by the
# IDE, or by --config here). One configure per toolchain then feeds all three
# configurations, and the artifacts land in Binaries/<CE_TOOLCHAIN_NAME>/<Config>/ so
# nothing overwrites anything else.

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

# Captured here because inside a zsh function $0 is the function name, not the script
# shellcheck disable=SC2034
SCRIPT_NAME="${0}"

# Every toolchain known to CMakePresets.json. The preset names are derived from these:
#   config-preset-<toolchain> / build-preset-<toolchain>-multi / test-preset-<toolchain>-multi
ALL_TOOLCHAINS=(clang appleclang xcode)
ALL_CONFIGS=(Debug Release Dist)

# Parse command line arguments
TOOLCHAINS=()
CONFIGS=()
RUN_TESTS=true
FRESH=false
CLEAN=false
FAIL_FAST=false
DRY_RUN=false
JOBS=""

print_usage() {
	echo "Usage: ${SCRIPT_NAME} [OPTIONS]"
	echo ""
	echo "Builds every toolchain x configuration combination in one go."
	echo ""
	echo "Options:"
	echo "  -t, --toolchain <name>   Only this toolchain (repeatable). Known: ${ALL_TOOLCHAINS[*]}"
	echo "  -c, --config <name>      Only this configuration (repeatable). Known: ${ALL_CONFIGS[*]}"
	echo "  -j, --jobs <n>           Parallel build jobs (default: generator's own choice)"
	echo "      --no-tests           Build only, skip ctest"
	echo "      --fresh              Re-run the configure step with --fresh (discards the cache)"
	echo "      --clean              Delete the build directory before configuring"
	echo "      --fail-fast          Stop at the first failing combination"
	echo "      --dry-run            Print the commands without running them"
	echo "  -l, --list               List the available toolchains and exit"
	echo "  -h, --help               Show this help message"
	echo ""
	echo "Examples:"
	echo "  ${SCRIPT_NAME}                                   # everything, with tests"
	echo "  ${SCRIPT_NAME} --no-tests                        # just check that all 9 combinations compile"
	echo "  ${SCRIPT_NAME} -t clang -c Debug -c Release      # narrow down while iterating"
}

while [[ $# -gt 0 ]]; do
	case $1 in
		-t|--toolchain)
			TOOLCHAINS+=("$2")
			shift 2
			;;
		-c|--config)
			CONFIGS+=("$2")
			shift 2
			;;
		-j|--jobs)
			JOBS="$2"
			shift 2
			;;
		--no-tests)
			RUN_TESTS=false
			shift
			;;
		--fresh)
			FRESH=true
			shift
			;;
		--clean)
			CLEAN=true
			shift
			;;
		--fail-fast)
			FAIL_FAST=true
			shift
			;;
		--dry-run)
			DRY_RUN=true
			shift
			;;
		-l|--list)
			LIST_ONLY=true
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

# Presets whose "condition" is not satisfied on this host (e.g. the Darwin-only ones on
# Linux) are absent from --list-presets, so that is the authority on what can run here
# rather than a hardcoded platform check.
AVAILABLE_PRESETS="$(cmake --list-presets 2>/dev/null)"
if [ -z "${AVAILABLE_PRESETS}" ]; then
	echo "${RED}Error: no usable configure preset found.${NC}"
	echo "${YELLOW}Run 'cmake --list-presets' in ${PROJECT_DIR} to see why.${NC}"
	exit 1
fi

AVAILABLE_TOOLCHAINS=()
for toolchain in "${ALL_TOOLCHAINS[@]}"; do
	if echo "${AVAILABLE_PRESETS}" | grep -q "\"config-preset-${toolchain}\""; then
		AVAILABLE_TOOLCHAINS+=("${toolchain}")
	fi
done

if [ "${LIST_ONLY}" = true ]; then
	echo "${BLUE}Available toolchains on this host:${NC}"
	for toolchain in "${AVAILABLE_TOOLCHAINS[@]}"; do
		echo "  ${toolchain}"
	done
	exit 0
fi

# No explicit selection means "everything that works here"
if [ ${#TOOLCHAINS[@]} -eq 0 ]; then
	TOOLCHAINS=("${AVAILABLE_TOOLCHAINS[@]}")
fi
if [ ${#CONFIGS[@]} -eq 0 ]; then
	CONFIGS=("${ALL_CONFIGS[@]}")
fi

# Validate the selection before doing any work, so a typo fails in a second instead of
# halfway through a full build
for toolchain in "${TOOLCHAINS[@]}"; do
	if [[ " ${ALL_TOOLCHAINS[*]} " != *" ${toolchain} "* ]]; then
		echo "${RED}Error: unknown toolchain '${toolchain}'. Known: ${ALL_TOOLCHAINS[*]}${NC}"
		exit 1
	fi
	if [[ " ${AVAILABLE_TOOLCHAINS[*]} " != *" ${toolchain} "* ]]; then
		echo "${RED}Error: toolchain '${toolchain}' is not available on this host${NC}"
		echo "${YELLOW}Its configure preset condition is not satisfied (see --list)${NC}"
		exit 1
	fi
done
for config in "${CONFIGS[@]}"; do
	if [[ " ${ALL_CONFIGS[*]} " != *" ${config} "* ]]; then
		echo "${RED}Error: unknown configuration '${config}'. Known: ${ALL_CONFIGS[*]}${NC}"
		exit 1
	fi
done

# The presets read the toolchain file and the Homebrew LLVM paths from the environment,
# so a login shell that never exported them would fail deep inside CMake
if [ -z "${CMAKE_TOOLCHAIN_FILE}" ]; then
	echo "${RED}Error: CMAKE_TOOLCHAIN_FILE is not set (the presets read it from the environment)${NC}"
	exit 1
fi
if [[ " ${TOOLCHAINS[*]} " == *" clang "* ]] && [ -z "${CLANG_CMAKE_CXX_COMPILER}" ]; then
	echo "${RED}Error: CLANG_CMAKE_CXX_COMPILER is not set, required by config-preset-clang${NC}"
	exit 1
fi

echo "${BLUE}========================================${NC}"
echo "${BLUE}CelestialEngine - Build All${NC}"
echo "${BLUE}========================================${NC}"
echo "Toolchains:     ${TOOLCHAINS[*]}"
echo "Configurations: ${CONFIGS[*]}"
if [ "$RUN_TESTS" = false ]; then
	echo "${YELLOW}Tests:          SKIPPED${NC}"
fi
if [ "$DRY_RUN" = true ]; then
	echo "${YELLOW}Mode:           DRY RUN${NC}"
fi
echo ""

# Result bookkeeping
STEPS_TOTAL=0
STEPS_FAILED=0
SUMMARY=()
ABORTED=false

# zsh resets SECONDS to 0 when the script starts, but capture it anyway so the total stays
# correct if this block ever moves
SCRIPT_START_TIME=$SECONDS

# Turns a number of seconds into 45s / 3m 07s / 1h 12m 30s
format_duration() {
	local total=$1
	local hours=$((total / 3600))
	local minutes=$(((total % 3600) / 60))
	local seconds=$((total % 60))

	if [ "$hours" -gt 0 ]; then
		printf '%dh %02dm %02ds' "$hours" "$minutes" "$seconds"
	elif [ "$minutes" -gt 0 ]; then
		printf '%dm %02ds' "$minutes" "$seconds"
	else
		printf '%ds' "$seconds"
	fi
}

# Runs one command, streaming its output, and records the outcome in the summary
run_step() {
	local label="$1"
	shift

	STEPS_TOTAL=$((STEPS_TOTAL + 1))

	echo "${BLUE}>>> ${label}${NC}"
	echo "${YELLOW}    $*${NC}"

	if [ "$DRY_RUN" = true ]; then
		SUMMARY+=("${YELLOW}~${NC} ${label} (dry run)")
		echo ""
		return 0
	fi

	local start_time=$SECONDS
	"$@"
	local exit_code=$?
	local elapsed
	elapsed="$(format_duration $((SECONDS - start_time)))"

	if [ $exit_code -eq 0 ]; then
		echo "${GREEN}✓ ${label} (${elapsed})${NC}"
		SUMMARY+=("${GREEN}✓${NC} ${label} (${elapsed})")
	else
		echo "${RED}✗ ${label} failed with exit code ${exit_code} (${elapsed})${NC}"
		SUMMARY+=("${RED}✗${NC} ${label} - exit ${exit_code}")
		STEPS_FAILED=$((STEPS_FAILED + 1))
	fi

	echo ""
	return $exit_code
}

for toolchain in "${TOOLCHAINS[@]}"; do
	[ "$ABORTED" = true ] && break

	config_preset="config-preset-${toolchain}"
	build_preset="build-preset-${toolchain}-multi"
	test_preset="test-preset-${toolchain}-multi"

	echo "${BLUE}========================================${NC}"
	echo "${BLUE}Toolchain: ${toolchain}${NC}"
	echo "${BLUE}========================================${NC}"
	echo ""

	# CMake exposes no way to query a preset's binaryDir, so --clean mirrors the
	# .build/<toolchain> convention of CMakePresets.json. Keep the two in sync.
	build_dir="${PROJECT_DIR}/.build/${toolchain}"

	if [ "$CLEAN" = true ]; then
		if [ -d "${build_dir}" ]; then
			echo "${YELLOW}Removing ${build_dir}${NC}"
			[ "$DRY_RUN" = false ] && rm -rf "${build_dir}"
		else
			echo "${YELLOW}Nothing to clean: ${build_dir} does not exist${NC}"
		fi
		echo ""
	fi

	# Configure once per toolchain: multi-config generators cover all three
	# configurations from a single cache
	configure_args=(cmake --preset "${config_preset}")
	[ "$FRESH" = true ] && configure_args+=(--fresh)

	if ! run_step "configure ${toolchain}" "${configure_args[@]}"; then
		echo "${YELLOW}Skipping the ${toolchain} builds: the configure step failed${NC}"
		echo ""
		[ "$FAIL_FAST" = true ] && ABORTED=true
		continue
	fi

	for config in "${CONFIGS[@]}"; do
		[ "$ABORTED" = true ] && break

		build_args=(cmake --build --preset "${build_preset}" --config "${config}")
		[ -n "${JOBS}" ] && build_args+=(--parallel "${JOBS}")

		if ! run_step "build ${toolchain}/${config}" "${build_args[@]}"; then
			[ "$FAIL_FAST" = true ] && ABORTED=true
			continue
		fi

		if [ "$RUN_TESTS" = true ]; then
			# ctest spells it -C, not --config
			if ! run_step "test ${toolchain}/${config}" \
				ctest --preset "${test_preset}" -C "${config}"; then
				[ "$FAIL_FAST" = true ] && ABORTED=true
			fi
		fi
	done
done

# Final summary
echo "${BLUE}========================================${NC}"
echo "${BLUE}Build All Summary${NC}"
echo "${BLUE}========================================${NC}"
echo ""

for line in "${SUMMARY[@]}"; do
	echo "  ${line}"
done
echo ""

if [ "$ABORTED" = true ]; then
	echo "${YELLOW}Stopped early (--fail-fast)${NC}"
	echo ""
fi

echo "${BLUE}Total time: $(format_duration $((SECONDS - SCRIPT_START_TIME)))${NC}"
echo ""

if [ $STEPS_FAILED -eq 0 ]; then
	echo "${GREEN}✓ All ${STEPS_TOTAL} steps succeeded${NC}"
	exit 0
else
	echo "${RED}✗ ${STEPS_FAILED}/${STEPS_TOTAL} steps failed${NC}"
	exit 1
fi
