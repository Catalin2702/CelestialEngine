#!/bin/zsh

#
# Module: CelestialEngine/Scripts/BuildSystem/XCode
# File: create_xcode_proj.sh
# Created by: Catalin Chirosca
# Created: 2026-04-18
# Updated by: Catalin Chirosca
# Updated: 2026-08-17
#

# Script to create an XCode project for the Celestial Engine
#
# Thin wrapper around the config-preset-xcode configure preset: that preset already uses
# the Xcode generator, so configuring it *is* generating the .xcodeproj. Keeping the
# generation in the preset (instead of a hand-written cmake command) means the IDE, this
# script and Scripts/BuildSystem/All/build_all.sh all share one definition of the Xcode
# build - including CE_TOOLCHAIN_NAME, which decides the Binaries/ output subfolder.

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

CONFIGURE_PRESET="config-preset-xcode"
XCODE_PROJECT="${PROJECT_DIR}/.build/xcode/CelestialEngine.xcodeproj"

# Parse command line arguments
FRESH=false
OPEN_PROJECT=false

print_usage() {
	echo "Usage: ${SCRIPT_NAME} [OPTIONS]"
	echo ""
	echo "Generates the XCode project through the ${CONFIGURE_PRESET} configure preset."
	echo ""
	echo "Options:"
	echo "      --fresh      Discard the existing CMake cache before generating"
	echo "  -o, --open       Open the generated project in XCode when done"
	echo "  -h, --help       Show this help message"
}

while [[ $# -gt 0 ]]; do
	case $1 in
		--fresh)
			FRESH=true
			shift
			;;
		-o|--open)
			OPEN_PROJECT=true
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

# The preset is Darwin-only (its "condition"), so on any other host it is simply absent
# from --list-presets rather than failing with a confusing CMake error
if ! cmake --list-presets 2>/dev/null | grep -q "\"${CONFIGURE_PRESET}\""; then
	echo "${RED}Error: preset ${CONFIGURE_PRESET} is not available on this host${NC}"
	echo "${YELLOW}The XCode generator requires macOS${NC}"
	exit 1
fi

if [ -z "${CMAKE_TOOLCHAIN_FILE}" ]; then
	echo "${RED}Error: CMAKE_TOOLCHAIN_FILE is not set (the preset reads it from the environment)${NC}"
	exit 1
fi

echo "${BLUE}Generating XCode project via ${CONFIGURE_PRESET}...${NC}"

configure_args=(cmake --preset "${CONFIGURE_PRESET}")
[ "$FRESH" = true ] && configure_args+=(--fresh)

if ! "${configure_args[@]}"; then
	echo "${RED}Failed to generate XCode project${NC}"
	exit 1
fi

if [ ! -d "${XCODE_PROJECT}" ]; then
	echo "${RED}Error: CMake succeeded but ${XCODE_PROJECT} is missing${NC}"
	echo "${YELLOW}Check the generator and binaryDir of ${CONFIGURE_PRESET}${NC}"
	exit 1
fi

echo ""
echo "${GREEN}XCode project generated successfully at ${XCODE_PROJECT}${NC}"

if [ "$OPEN_PROJECT" = true ]; then
	echo "${BLUE}Opening in XCode...${NC}"
	open "${XCODE_PROJECT}"
fi
