#!/bin/bash

cd "$(dirname "$0")" || exit

echo "🔧 Generating icons for CelestialEngine..."

# Determine which image to use for light mode
if [ $# -eq 1 ]; then
    INPUT="$1"
    echo "📂 Using custom image: $INPUT"
else
    # Use the Default image from Exports folder
    INPUT="CelestialEngineExports/CelestialEngine-iOS-Default-1024x1024@1x.png"
    echo "📂 Using Default image"
fi

# Dark mode source
INPUT_DARK="CelestialEngineExports/CelestialEngine-iOS-Dark-1024x1024@1x.png"

# Verify that the file exists
if [ ! -f "$INPUT" ]; then
    echo "❌ Error: File '$INPUT' not found"
    exit 1
fi

# Output directory
OUTPUT_DIR="Assets.xcassets/AppIcon.appiconset"

echo "📐 Generating Light Mode icon sizes..."

# Generate all sizes required by macOS (Light Mode)
sips -z 16 16 "$INPUT" --out "$OUTPUT_DIR/icon_16x16.png" > /dev/null 2>&1
sips -z 32 32 "$INPUT" --out "$OUTPUT_DIR/icon_16x16@2x.png" > /dev/null 2>&1
sips -z 32 32 "$INPUT" --out "$OUTPUT_DIR/icon_32x32.png" > /dev/null 2>&1
sips -z 64 64 "$INPUT" --out "$OUTPUT_DIR/icon_32x32@2x.png" > /dev/null 2>&1
sips -z 128 128 "$INPUT" --out "$OUTPUT_DIR/icon_128x128.png" > /dev/null 2>&1
sips -z 256 256 "$INPUT" --out "$OUTPUT_DIR/icon_128x128@2x.png" > /dev/null 2>&1
sips -z 256 256 "$INPUT" --out "$OUTPUT_DIR/icon_256x256.png" > /dev/null 2>&1
sips -z 512 512 "$INPUT" --out "$OUTPUT_DIR/icon_256x256@2x.png" > /dev/null 2>&1
sips -z 512 512 "$INPUT" --out "$OUTPUT_DIR/icon_512x512.png" > /dev/null 2>&1
sips -z 1024 1024 "$INPUT" --out "$OUTPUT_DIR/icon_512x512@2x.png" > /dev/null 2>&1

echo "✅ Light Mode sizes generated!"

# Generate Dark Mode variants if source exists
if [ -f "$INPUT_DARK" ]; then
    echo "🌙 Generating Dark Mode icon sizes..."

    sips -z 16 16 "$INPUT_DARK" --out "$OUTPUT_DIR/icon_16x16_dark.png" > /dev/null 2>&1
    sips -z 32 32 "$INPUT_DARK" --out "$OUTPUT_DIR/icon_16x16@2x_dark.png" > /dev/null 2>&1
    sips -z 32 32 "$INPUT_DARK" --out "$OUTPUT_DIR/icon_32x32_dark.png" > /dev/null 2>&1
    sips -z 64 64 "$INPUT_DARK" --out "$OUTPUT_DIR/icon_32x32@2x_dark.png" > /dev/null 2>&1
    sips -z 128 128 "$INPUT_DARK" --out "$OUTPUT_DIR/icon_128x128_dark.png" > /dev/null 2>&1
    sips -z 256 256 "$INPUT_DARK" --out "$OUTPUT_DIR/icon_128x128@2x_dark.png" > /dev/null 2>&1
    sips -z 256 256 "$INPUT_DARK" --out "$OUTPUT_DIR/icon_256x256_dark.png" > /dev/null 2>&1
    sips -z 512 512 "$INPUT_DARK" --out "$OUTPUT_DIR/icon_256x256@2x_dark.png" > /dev/null 2>&1
    sips -z 512 512 "$INPUT_DARK" --out "$OUTPUT_DIR/icon_512x512_dark.png" > /dev/null 2>&1
    sips -z 1024 1024 "$INPUT_DARK" --out "$OUTPUT_DIR/icon_512x512@2x_dark.png" > /dev/null 2>&1

    echo "✅ Dark Mode sizes generated!"
else
    echo "⚠️  Dark Mode source image not found: $INPUT_DARK"
    echo "   Skipping Dark Mode icon generation"
fi

echo ""
echo "📋 Files generated in: $OUTPUT_DIR"
echo ""
echo "🔨 To apply changes:"
echo "   cd ../../Binaries/Debug"
echo "   make CE_App"
echo ""
echo "💡 Tip: You can also use Dark/Light/Tinted variants by modifying Contents.json"
echo "   See ICON_GUIDE.md for more details"
