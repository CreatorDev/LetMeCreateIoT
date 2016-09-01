#!/bin/bash

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

if [[ ! -d "$CONTIKI/$LIBRARY_DIR_NAME" ]]; then
    echo "Could not find directory $LIBRARY_DIR_NAME. Check your contiki path"
    exit 1
fi

if [[ -d $LMC_DIR ]]; then
    rm -rf $LMC_DIR
fi

mkdir -p $LMC_DIR

cp -r $SRC_DIR/* $LMC_DIR/
cp -r $INCLUDE_DIR/* $LMC_DIR/


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

grep --quiet -P "MODULES(.*?)$LIBRARY_DIR_NAME/$LMC_DIR_NAME/3rd_party" $MAKEFILE
if [[ $? -ne 0 ]]; then
    echo "Adding LMC 3rd party module to Contiki makefile..."
    sed -i "0,/MODULES +=/s//MODULES += ${LIBRARY_DIR_NAME}\/${LMC_DIR_NAME}\/3rd_party/" $MAKEFILE
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
