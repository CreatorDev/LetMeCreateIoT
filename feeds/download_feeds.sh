#!/bin/bash

function realpath() {
    cd -P "$1"
    echo "$PWD"
    cd - > /dev/null
}

function download_feeds {
    local FEEDS_FILE="$BASE_DIR/feeds/feed"
    local FEEDS_REPO="https://github.com/CreatorDev/LetMeCreate"
    local FEEDS_DIR="$BASE_DIR/feeds/tmp"

    if [[ ! -e "$FEEDS_FILE" ]]; then
        echo "Could not find the feed tag file"
        return 1
    fi

    local TAG=$(cat "$FEEDS_FILE")

    if [[ -d "$FEEDS_DIR" ]]; then
        echo "feeds_tmp exists, removing"
        rm -rf "$FEEDS_DIR"
    fi

    git clone "$FEEDS_REPO" "$FEEDS_DIR"
    if [[ $? -ne 0 ]]; then
        echo "Failed to clone feeds repo"
        return 1
    fi

    cd "$FEEDS_DIR"
    git checkout "$TAG" > /dev/null 2>&1

    if [[ $? -ne 0 ]]; then
        echo "Failed to checkout $TAG"
        return 1
    fi

    cp "$BASE_DIR/feeds/patches/"*.patch "$FEEDS_DIR/" 2>/dev/null
    if [[ $? -eq 0 ]]; then
        for x in *.patch
        do
            echo "Applying patch $x"
            git apply "$x"
            if [[ $? -ne 0 ]]; then
                echo "Patch failed to apply, aborting"
                return 1
            fi
        done
    fi

    # Clear folders we do not want
    for I in "${FEEDS_SKIP_FOLDERS[@]}"; do
        echo "Removing $I"
        rm -rf "$FEEDS_DIR/include/letmecreate/$I"
        rm -rf "$FEEDS_DIR/src/$I"
    done
    # Clear headers and core files
    rm -rf "$FEEDS_DIR/include/letmecreate/core"
    rm -rf "$FEEDS_DIR/include/letmecreate"/*.h
    rm -rf "$FEEDS_DIR/src/core"

    cd "$BASE_DIR"
    # Copy filtered files over
    cp -r "$FEEDS_DIR/include/letmecreate"/* "$BASE_DIR/letmecreate/"
    cp -r "$FEEDS_DIR/src"/* "$BASE_DIR/src/"
    rm -rf "$FEEDS_DIR"

    cat "$FEEDS_FILE" > "$BASE_DIR/feeds/cloned"
}

BASE_DIR=$(realpath $(dirname $0)/../)
FEEDS_SKIP_FOLDERS=( rpisensehat )

download_feeds
