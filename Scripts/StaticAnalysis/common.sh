#!/bin/zsh

#
# Module: CelestialEngine/Scripts/StaticAnalysis
# File: common.sh
# Created by: Catalin Chirosca
# Created: 2026-09-18
# Updated by: Catalin Chirosca
# Updated: 2026-09-18
#

# Shared helpers for the static analysis scripts. Sourced, never executed directly.
#
# Every analyzer here is driven by a compile_commands.json, so it sees exactly the defines, include paths and
# language standard the real build uses. The Ninja Multi-Config generator does not write one by itself, so it is
# extracted from the per-configuration ninja file of an existing build directory (ninja -t compdb) and then filtered
# down to the project's own sources.

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Base directories. ${(%):-%x} is the path of the sourced file itself, not of the script that sourced it.
STATIC_ANALYSIS_DIR="${${(%):-%x}:A:h}"
PROJECT_DIR="${STATIC_ANALYSIS_DIR:h:h}"
REPORTS_ROOT="${PROJECT_DIR}/.build/static-analysis"

# Only the Ninja generators have a ninja file to extract the commands from; the Xcode project does not.
ANALYSIS_TOOLCHAINS=(clang appleclang)
ALL_CONFIGS=(Debug Release Dist)

# Resolves a tool from Homebrew's LLVM first (it matches the compiler of the clang preset), then from PATH.
ce_find_llvm_tool() {
	local tool="$1"
	local candidate
	for candidate in \
		"${CLANG_CMAKE_CXX_COMPILER:h}/${tool}" \
		"$(brew --prefix llvm 2>/dev/null)/bin/${tool}" \
		"$(command -v "${tool}" 2>/dev/null)"; do
		if [ -n "${candidate}" ] && [ -x "${candidate}" ]; then
			echo "${candidate}"
			return 0
		fi
	done
	return 1
}

# ce_make_compdb <toolchain> <config> <out_dir> [with_objcxx]
#
# Writes <out_dir>/compile_commands.json for the project's own translation units. The build directory is configured
# through its preset first when it does not exist yet. The build does not need to have run: the commands are read
# from the ninja file, not from the compiled objects.
ce_make_compdb() {
	local toolchain="$1" config="$2" out_dir="$3" with_objcxx="${4:-true}"
	local build_dir="${PROJECT_DIR}/.build/${toolchain}"
	local ninja_file="${build_dir}/build-${config}.ninja"

	if [ ! -f "${ninja_file}" ]; then
		echo "${YELLOW}No ${ninja_file:t} in ${build_dir}, configuring config-preset-${toolchain}...${NC}"
		(cd "${PROJECT_DIR}" && cmake --preset "config-preset-${toolchain}" >/dev/null) || {
			echo "${RED}Error: configuring config-preset-${toolchain} failed${NC}"
			return 1
		}
	fi

	mkdir -p "${out_dir}"
	local raw="${out_dir}/compile_commands.raw.json"
	(cd "${build_dir}" && ninja -f "build-${config}.ninja" -t compdb > "${raw}") || {
		echo "${RED}Error: ninja -t compdb failed in ${build_dir}${NC}"
		return 1
	}

	python3 - "${raw}" "${out_dir}/compile_commands.json" "${PROJECT_DIR}" "${with_objcxx}" <<'EOF' || return 1
import json, os, shlex, sys

raw, out, root, with_objcxx = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4] == "true"
extensions = (".cpp", ".cc", ".cxx") + ((".mm",) if with_objcxx else ())
owned = tuple(os.path.join(root, d) + os.sep for d in ("Engine/Modules", "App", "Tests"))

entries = []
for entry in json.load(open(raw)):
    source = entry["file"]
    # clang-scan-deps entries are the C++ module dependency scan, not a compilation.
    if "clang-scan-deps" in entry["command"] or not source.endswith(extensions) or not source.startswith(owned):
        continue
    args = shlex.split(entry["command"])
    kept = []
    for arg in args:
        # CMake's module map response file is empty while no C++ module is declared, and tools that do not expand
        # response files choke on it; drop it only in that case.
        if arg.startswith("@") and arg.endswith(".modmap"):
            path = os.path.join(entry["directory"], arg[1:])
            if not os.path.exists(path) or os.path.getsize(path) == 0:
                continue
        kept.append(arg)
    entries.append({"directory": entry["directory"], "file": source, "arguments": kept})

json.dump(entries, open(out, "w"), indent=1)
print(f"{len(entries)} translation units")
EOF
	rm -f "${raw}"
}

# ce_validate_selection <toolchain> <config>
ce_validate_selection() {
	local toolchain="$1" config="$2"
	if [[ " ${ANALYSIS_TOOLCHAINS[*]} " != *" ${toolchain} "* ]]; then
		echo "${RED}Error: toolchain '${toolchain}' cannot be analyzed. Known: ${ANALYSIS_TOOLCHAINS[*]}${NC}"
		echo "${YELLOW}The xcode preset has no ninja file to extract the compile commands from.${NC}"
		return 1
	fi
	if [[ " ${ALL_CONFIGS[*]} " != *" ${config} "* ]]; then
		echo "${RED}Error: unknown configuration '${config}'. Known: ${ALL_CONFIGS[*]}${NC}"
		return 1
	fi
}
