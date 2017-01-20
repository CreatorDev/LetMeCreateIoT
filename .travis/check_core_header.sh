#!/bin/bash

BASE_DIR=$(dirname $(readlink -f $0))

HEADERS=$(ls "$BASE_DIR/../include/core/")

echo "Checking if all core headers are included in the header"
CODE=0
for x in $HEADERS; do
    echo "Checking if core.h includes $x"
    grep -E --quiet "$x" "$BASE_DIR/../include/core.h"
    if [[ $? -ne 0 ]]; then
        CODE=1
        echo "Missing!"
    fi
done

exit $CODE
