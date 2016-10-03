#!/bin/bash

EXAMPLES_DIR=$(dirname $(readlink -f $0))/../examples

FAILS=()
for dir in $EXAMPLES_DIR/*; do
    EXAMPLE_NAME=$(basename $dir)
    echo "*** Building example $EXAMPLE_NAME ***"
    cd $dir
    make
    if [[ $? -ne 0 ]]; then
        echo "Build of $EXAMPLE_NAME failed"
        FAILS+=($EXAMPLE_NAME)
    fi

    ls *.hex 1>/dev/null 2>&1
    if [[ $? -ne 0 ]]; then
        echo "No output hex file found for $EXAMPLE_NAME"
        FAILS+=($EXAMPLE_NAME)
    fi
done

if [[ ${#FAILS[@]} > 0 ]]; then
    echo "Examples failed to build: ${FAILS[@]}"
    exit 1
fi
