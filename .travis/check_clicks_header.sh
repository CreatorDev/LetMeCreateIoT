#!/bin/bash

BASE_DIR=$(dirname $(readlink -f $0))

CLICKS=$(ls -I "ft800_defs.h" -I "common.h" -I "export.h" "$BASE_DIR/../letmecreate/click/")

if [[ $? -ne 0 ]]; then
    echo "Failed to look through directory"
    exit 1
fi

echo "Checking if clicks are included in the header"
CODE=0
for x in $CLICKS; do
    echo "Checking if click.h includes $x"
    grep -E --quiet "$x" "$BASE_DIR/../letmecreate/click.h"
    if [[ $? -ne 0 ]]; then
        CODE=1
        echo "Click.h file does not include $x file"
    fi
done

exit $CODE
