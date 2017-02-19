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
    echo "-s: Skips the feed downloads and installs files stored locally"
    echo "-u: Uninstalls the library from Contiki folder"
}

realpath() {
    cd -P "$1"
    echo "$PWD"
    cd - > /dev/null
}

function install_files {
    if [[ -d "$LMC_DIR" ]]; then
        rm -rf "$LMC_DIR"
    fi

    mkdir -p "$LMC_DIR"

    cp -r "$SRC_DIR"/* "$LMC_DIR/"
    cp -r "$INCLUDE_DIR"/* "$LMC_DIR/"

    if [[ -d "$STAGING_DIR" ]]; then
        cp -r "$STAGING_DIR"/* "$LMC_DIR/"
    fi

    local MAKEFILE="$CONTIKI/Makefile.include"
    local PLATFORM_MAIN="$CONTIKI/platform/mikro-e/contiki-mikro-e-main.c"

    echo "Checking if Makefile patch needs to be applied"
    grep -E --quiet "MODULES(.*?)$LIBRARY_DIR_NAME/$LMC_DIR_NAME/core" "$MAKEFILE"
    if [[ $? -eq 1 ]]; then
        echo "Adding LMC files"
        cd "$CONTIKI"
        git apply "$BASE_DIR/patches/makefile.patch" || exit 1
        cd - > /dev/null
    fi

    echo "Checking if should remove ISR calls..."
    grep -E --quiet "CHANGE_NOTICE_VECTOR" "$PLATFORM_MAIN"
    if [[ $? -eq 0 ]]; then
        echo "Removing ISR from contiki-mikro-e-main.c"
        cd "$CONTIKI/platform/mikro-e"
        git apply "$BASE_DIR/patches/mikro-e.patch" || exit 1
        cd - > /dev/null
    fi

    if [[ -d "$CONTIKI_SYMLINK" ]]; then
        unlink "$CONTIKI_SYMLINK"
    fi

    echo "Creating symbolic link..."
    if [[ ! -d "$CONTIKI_SYMLINK" ]]; then
        ln -s "$CONTIKI" "$CONTIKI_SYMLINK"
    fi

    if [[ $? -ne 0 ]]; then
        echo "Failed to create symlink, continuing..."
    fi


    echo "Library installed successfully"

    return 0
}

function download_feeds {
    local FEEDS_FILE="$BASE_DIR/feeds/feed"
    local FEEDS_REPO="https://github.com/CreatorDev/LetMeCreate"

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

    # Clear headers and core files
    rm -rf "$FEEDS_DIR/include/letmecreate/core"
    rm -rf "$FEEDS_DIR/include/letmecreate"/*.h
    rm -rf "$FEEDS_DIR/src/core"

    # Clear files that match the exclude regex
    for I in "${EXCLUDED[@]}"; do
        echo "Removing files matching regex '$I'"
        cd "$FEEDS_DIR/include/letmecreate/"
        find ./ -wholename "$I" -type f | xargs rm -f

        cd "$FEEDS_DIR/src/"
        find ./ -wholename "$I" -type f | xargs rm -f
    done

    cd "$BASE_DIR"
    # Copy filtered files over
    cp -r "$FEEDS_DIR/include/letmecreate"/* "$STAGING_DIR/"
    cp -r "$FEEDS_DIR/src"/* "$STAGING_DIR/"
    cp -r "$FEEDS_DIR/include/letmecreate"/* "$BASE_DIR/include/"
    cp -r "$FEEDS_DIR/src"/* "$BASE_DIR/src/"
    rm -rf "$FEEDS_DIR"
}

function copy_local_files_to_staging {
    cp -r "$BASE_DIR/include"/* "$STAGING_DIR/"
    cp -r "$BASE_DIR/src"/* "$STAGING_DIR/"
}

function uninstall {
    if [[ ! -L "$CONTIKI_SYMLINK" ]]; then
        echo "No Contiki symlink found, uninstall aborted"
        return 1
    fi


    echo "Removing LMC directory"
    rm -rf "$CONTIKI_SYMLINK/$LIBRARY_DIR_NAME/$LMC_DIR_NAME"

    if [[ $? -ne 0 ]]; then
        echo "Failed to remove the LMC dir"
        return 1
    fi
    local MAKEFILE="$CONTIKI_SYMLINK/Makefile.include"

    echo "Removing patches"
    cd "$CONTIKI_SYMLINK"
    for x in "$BASE_DIR/patches/"*.patch; do
        git apply -R "$x" || exit 1
    done
    cd - > /dev/null

    echo "Removing symlink"
    unlink "$CONTIKI_SYMLINK"
}

EXCLUDED=()
SKIP_CHECKOUT=false
BASE_DIR=$(realpath $(dirname $0))
CONTIKI_SYMLINK="$BASE_DIR/contiki"
LIBRARY_DIR_NAME="core"
LMC_DIR_NAME="letmecreate"

while getopts ":e:p:su" opt; do
    case $opt in
        e)
            EXCLUDED+=("$OPTARG")
            ;;
        p)
            CONTIKI=$(realpath "$OPTARG")
            if [[ -d "$CONTIKI_SYMLINK" ]]; then
                unlink "$CONTIKI_SYMLINK"
            fi
            ;;
        s)
            SKIP_CHECKOUT=true
            ;;
        h)
            print_help
            exit 1
            ;;
        u)
            uninstall
            if [[ $? -ne 0 ]]; then
                echo "Uninstall failed"
                exit 1
            fi
            echo "Uninstall finished"
            exit 0
            ;;
        \?)
            echo "Unrecognised option: -$OPTARG" >&2
            print_help
            exit 1
            ;;
    esac
done


if [[ -z "$CONTIKI" ]]; then
    if [[ -d "$CONTIKI_SYMLINK" ]]; then
        echo "Detected contiki directory in the repository at $CONTIKI_SYMLINK"
        cd -P "$CONTIKI_SYMLINK"
        CONTIKI=$(pwd)
        cd "$BASE_DIR"
    else
        echo "No symlink detected (is this your first install?). Try: $0 -p contiki_dir"
        exit 1
    fi
fi


echo "Installing the library files to $CONTIKI"

if [[ ${#EXCLUDED[@]} -gt 0 ]]; then
    echo "Excluding regexes: ${EXCLUDED[@]}"
fi

LMC_DIR="$CONTIKI/$LIBRARY_DIR_NAME/$LMC_DIR_NAME"
SRC_DIR="$BASE_DIR/src"
INCLUDE_DIR="$BASE_DIR/include"
STAGING_DIR="$BASE_DIR/staging_dir"
FEEDS_DIR="$BASE_DIR/feeds_tmp"

if [[ ! -d "$CONTIKI/$LIBRARY_DIR_NAME" ]]; then
    echo "Could not find directory $LIBRARY_DIR_NAME. Check your contiki path"
    exit 1
fi

mkdir -p "$STAGING_DIR"
echo "Creating staging dir"

if [[ "$SKIP_CHECKOUT" = false ]]; then
    download_feeds
else
    copy_local_files_to_staging
fi

if [[ $? -ne 0 ]]; then
    rm -rf "$STAGING_DIR"
    echo "Failed to move files to staging"
    exit 1
fi

install_files
if [[ $? -ne 0 ]]; then
    rm -rf "$STAGING_DIR"
    rm -rf "$FEEDS_DIR"
    echo "Install failed!"
    exit 1
fi

rm -rf "$STAGING_DIR"
echo "Cleared staging dir"
