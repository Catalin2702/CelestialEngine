#!/bin/zsh

#
# Module: CelestialEngine/Scripts/BuildSystem/XCode
# File: create_xcode_proj.sh
# Created by: Catalin Chirosca
# Created: 2026-04-18
# Updated by: Catalin Chirosca
# Updated: 2026-04-18
#

# Script to create an XCode project for the Celestial Engine
# Uses CMake to generate the XCode project files
# The generated project will be located in the Build/XCode folder

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Base directories
SCRIPT_DIR="${0:A:h}"
PROJECT_DIR="${SCRIPT_DIR}/../../.."
BUILD_DIR="${PROJECT_DIR}/build-xcode"

# Create build directory if it doesn't exist
mkdir -p "${BUILD_DIR}"

# Generate XCode project using CMake
echo -e "${BLUE}Generating XCode project...${NC}"
cd "${PROJECT_DIR}" || exit 1

# Use build-xcode's own vcpkg_installed directory to be independent from regular build
cmake -B "${BUILD_DIR}" -G "Xcode" \
    -DCMAKE_TOOLCHAIN_FILE="$(pwd)/vcpkg_link/scripts/buildsystems/vcpkg.cmake" \
    -DVCPKG_TARGET_TRIPLET="arm64-osx"

if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to generate XCode project${NC}"
    exit 1
fi

echo -e "${GREEN}XCode project generated successfully at ${BUILD_DIR}${NC}"
