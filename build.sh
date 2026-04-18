#!/usr/bin/env bash
set -euo pipefail

CONFIG="${1:-Debug}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC="$ROOT/src"
BUILD="$ROOT/build"

echo "=== Conan install ==="
conan install "$SRC" --output-folder="$BUILD" --build=missing -s build_type="$CONFIG" || true

echo "=== CMake configure ==="
cmake -S "$SRC" -B "$BUILD" \
    -DCMAKE_TOOLCHAIN_FILE="$BUILD/conan_toolchain.cmake" \
    -DCMAKE_PREFIX_PATH="$BUILD"

echo "=== CMake build ==="
cmake --build "$BUILD" --config "$CONFIG"

echo "=== Build complete ($CONFIG) ==="
