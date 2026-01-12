#!/usr/bin/env bash

set -e

BUILD_DIR="${BUILD_DIR:-build}"
CONFIG="${CONFIG:-Release}"

command="${1:-run}"

configure() {
    cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$CONFIG"
}

build() {
    cmake --build "$BUILD_DIR" --config "$CONFIG"
}

run_app() {
    local exe=""
    if [[ -f "$BUILD_DIR/$CONFIG/flock.exe" ]]; then
        exe="$BUILD_DIR/$CONFIG/flock.exe"
    elif [[ -f "$BUILD_DIR/$CONFIG/flock" ]]; then
        exe="$BUILD_DIR/$CONFIG/flock"
    elif [[ -f "$BUILD_DIR/flock.exe" ]]; then
        exe="$BUILD_DIR/flock.exe"
    elif [[ -f "$BUILD_DIR/flock" ]]; then
        exe="$BUILD_DIR/flock"
    fi

    if [[ -z "$exe" ]]; then
        echo "Executable not found. Run './build.sh build' first."
        exit 1
    fi

    echo "Running $exe"
    "$exe"
}

case "$command" in
    clean)
        rm -rf "$BUILD_DIR"
        ;;
    build)
        configure
        build
        ;;
    rebuild)
        rm -rf "$BUILD_DIR"
        configure
        build
        ;;
    run)
        configure
        build
        run_app
        ;;
    help|-h|--help)
        echo "Usage: ./build.sh [clean|build|rebuild|run]"
        echo "  clean   - remove build directory"
        echo "  build   - configure and build"
        echo "  rebuild - clean, configure, build"
        echo "  run     - configure, build, and run (default)"
        ;;
    *)
        echo "Unknown command: $command"
        exit 1
        ;;
esac
