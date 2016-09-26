#!/bin/bash

function install_files {
    if [[ -d $LMC_DIR ]]; then
        rm -rf $LMC_DIR
    fi

    mkdir -p $LMC_DIR

    cp -r $SRC_DIR/* $LMC_DIR/
    cp -r $INCLUDE_DIR/* $LMC_DIR/

    if [[ -d "$STAGING_DIR" ]]; then
        cp -r $STAGING_DIR/* $LMC_DIR/
    fi

    MAKEFILE=$CONTIKI/Makefile.include

    grep --quiet -P "MODULES(.*?)$LIBRARY_DIR_NAME/$LMC_DIR_NAME/core" $MAKEFILE
    if [[ $? -ne 0 ]]; then
        echo "Adding LMC core module to Contiki makefile..."
        sed -i "0,/MODULES +=/s//MODULES += ${LIBRARY_DIR_NAME}\/${LMC_DIR_NAME}\/core/" $MAKEFILE
    fi

    grep --quiet -P "MODULES(.*?)$LIBRARY_DIR_NAME/$LMC_DIR_NAME/click" $MAKEFILE
    if [[ $? -ne 0 ]]; then
        echo "Adding LMC click module to Contiki makefile..."
        sed -i "0,/MODULES +=/s//MODULES += ${LIBRARY_DIR_NAME}\/${LMC_DIR_NAME}\/click/" $MAKEFILE
    fi

    if [[ -d "$CONTIKI_SYMLINK" ]]; then
        unlink "$CONTIKI_SYMLINK"
    fi

    echo "Creating symbolic link..."
    ln -s "$CONTIKI" "$CONTIKI_SYMLINK"
    if [[ $? -ne 0 ]]; then
        echo "Failed to create symlink, continuing..."
    fi

    echo "Library installed successfully"

    return 0
}

function download_feeds {
    local FEEDS_FILE="$BASE_DIR/feed"
    local FEEDS_DIR="$BASE_DIR/feeds_tmp"
    local FEEDS_REPO="https://github.com/francois-berder/LetMeCreate"

    if [[ ! -e $FEEDS_FILE ]]; then
        echo "Could not find the feed tag file"
        return 1
    fi

    local TAG=$(cat $FEEDS_FILE)

    git clone $FEEDS_REPO $FEEDS_DIR
    if [[ $? -ne 0 ]]; then
        echo "Failed to clone feeds repo"
        return 1
    fi

    cd $FEEDS_DIR
    git checkout $TAG > /dev/null 2>&1

    if [[ $? -ne 0 ]]; then
        echo "Failed to checkout $TAG"
        return 1
    fi

    cp $BASE_DIR/feeds/patches/*.patch $FEEDS_DIR/
    for x in *.patch
    do
        echo "Applying patch $x"
        git apply $x
    done

    rm -rf $FEEDS_DIR/include/letmecreate/core
    rm -rf $FEEDS_DIR/include/letmecreate/*.h
    rm -rf $FEEDS_DIR/src/core

    cp -r $FEEDS_DIR/include/letmecreate/* $STAGING_DIR/
    cp -r $FEEDS_DIR/src/* $STAGING_DIR/
    cp -r $FEEDS_DIR/include/letmecreate/* $BASE_DIR/include/
    cp -r $FEEDS_DIR/src/* $BASE_DIR/src/
    rm -rf $FEEDS_DIR
}

BASE_DIR=$(dirname $(readlink -f $0))
CONTIKI_SYMLINK="$BASE_DIR/contiki"

if [[ $# -ne 1 ]]; then
    if [[ -d "$CONTIKI_SYMLINK" ]]; then
        echo "Detected contiki directory in the repository at $CONTIKI_SYMLINK"
        CONTIKI=$(readlink -f "$CONTIKI_SYMLINK")
    else
        echo "Usage: $0 contiki_dir"
        exit 1
    fi
else
    CONTIKI=$(readlink -f $1)
fi

echo "Installing the library files to $CONTIKI"

LIBRARY_DIR_NAME="core"
LMC_DIR_NAME="letmecreate"
LMC_DIR="$CONTIKI/$LIBRARY_DIR_NAME/$LMC_DIR_NAME"
SRC_DIR="$BASE_DIR/src"
INCLUDE_DIR="$BASE_DIR/include"
STAGING_DIR="$BASE_DIR/staging_dir"

if [[ ! -d "$CONTIKI/$LIBRARY_DIR_NAME" ]]; then
    echo "Could not find directory $LIBRARY_DIR_NAME. Check your contiki path"
    exit 1
fi

mkdir -p $STAGING_DIR
echo "Creating staging dir"

download_feeds
if [[ $? -ne 0 ]]; then
    echo "Failed to get feeds!"
    exit 1
fi

install_files
if [[ $? -ne 0 ]]; then
    echo "Install failed!"
    exit 1
fi

rm -rf $STAGING_DIR
echo "Cleared staging dir"
