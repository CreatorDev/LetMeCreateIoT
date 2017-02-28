#!/bin/bash

BASE_DIR=$(dirname $(readlink -f $0))

HEADERS=$(ls "$BASE_DIR/../letmecreate/core/")

if [[ $? -ne 0 ]]; then
    echo "Failed to look through directory"
    exit 1
fi

echo "Checking if all core headers are included in the header"
CODE=0
for x in $HEADERS; do
    echo "Checking if core.h includes $x"
    grep -E --quiet "$x" "$BASE_DIR/../letmecreate/core.h"
    if [[ $? -ne 0 ]]; then
        CODE=1
        echo "Missing!"
    fi
done

exit $CODE
