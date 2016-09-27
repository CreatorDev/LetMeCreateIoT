#!/bin/bash

function print_help {
    echo "Usage: $0 -p contiki_path -e exclude_regex"
    echo "When installing the driver feeds will be copied into the local repo to allow for" \
         "documentation generation with doxygen. When excluding/including different drivers" \
         "remember to clean your repo first"
    echo "-h: Show help"
    echo "-p: Path where the library will be installed (required on first install)"
    echo "-e: Regex to use for the find command when excluding headers and source files." \
         "The regex applies to the filename combined with the top level folder it's in," \
         "i.e. './click/*' will match all files in the click directory, while '*relay.*' will" \
         "match 'click/relay.c' and 'click/relay.h'"
    echo "-s: Skips the feed downloads and installs files stored only locally"
}

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

    # Clear headers and core files
    rm -rf $FEEDS_DIR/include/letmecreate/core
    rm -rf $FEEDS_DIR/include/letmecreate/*.h
    rm -rf $FEEDS_DIR/src/core

    # Clear files that match the exclude regex
    for I in "${EXCLUDED[@]}"; do
        echo "Removing files matching regex '$I'"
        cd $FEEDS_DIR/include/letmecreate/
        find ./ -wholename "$I" -type f | xargs rm -f

        cd $FEEDS_DIR/src/
        find ./ -wholename "$I" -type f | xargs rm -f
    done

    cd $BASE_DIR
    # Copy filtered files over
    cp -r $FEEDS_DIR/include/letmecreate/* $STAGING_DIR/
    cp -r $FEEDS_DIR/src/* $STAGING_DIR/
    cp -r $FEEDS_DIR/include/letmecreate/* $BASE_DIR/include/
    cp -r $FEEDS_DIR/src/* $BASE_DIR/src/
    rm -rf $FEEDS_DIR
}

function copy_local_files_to_staging {
    cp -r $BASE_DIR/include/* $STAGING_DIR/
    cp -r $BASE_DIR/src/* $STAGING_DIR/
}

EXCLUDED=()
SKIP_CHECKOUT=false

while getopts ":e:p:s" opt; do
    case $opt in
        e)
            EXCLUDED+=("$OPTARG")
            ;;
        p)
            CONTIKI=$(readlink -f "$OPTARG")
            ;;
        s)
            SKIP_CHECKOUT=true
            ;;
        h)
            print_help
            exit 1
            ;;
        \?)
            echo "Unrecognised option: -$OPTARG" >&2
            print_help
            exit 1
            ;;
    esac
done

BASE_DIR=$(dirname $(readlink -f $0))
CONTIKI_SYMLINK="$BASE_DIR/contiki"

if [[ -z $CONTIKI ]]; then
    if [[ -d "$CONTIKI_SYMLINK" ]]; then
        echo "Detected contiki directory in the repository at $CONTIKI_SYMLINK"
        CONTIKI=$(readlink -f "$CONTIKI_SYMLINK")
    else
        echo "No symlink detected (is this your first install?). Try: $0 -p contiki_dir"
        exit 1
    fi
fi

echo "Installing the library files to $CONTIKI"

if [[ ${#EXCLUDED[@]} -gt 0 ]]; then
    echo "Excluding regexes: ${EXCLUDED[@]}"
fi

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

if [[ "$SKIP_CHECKOUT" = false ]]; then
    download_feeds
else
    copy_local_files_to_staging
fi

if [[ $? -ne 0 ]]; then
    echo "Failed to move files to staging"
    exit 1
fi

install_files
if [[ $? -ne 0 ]]; then
    echo "Install failed!"
    exit 1
fi

rm -rf $STAGING_DIR
echo "Cleared staging dir"
