#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 contiki_dir"
    exit 1
fi

BASE_DIR=$(dirname $(readlink -f $0))
CONTIKI=$(readlink -f $1)

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


if [[ $? -ne 0 ]]; then
    echo "Failed to modify the makefile"
    exit 1
fi

echo "Library installed successfully"
