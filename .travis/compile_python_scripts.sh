#!/bin/bash

BASE_DIR=$(dirname $(readlink -f $0))/..

find $BASE_DIR/ -wholename "*.py" | xargs python3 -m py_compile

if [[ $? -ne 0 ]]; then
    echo "Compilation of at least one script failed"
    exit 1
fi

echo "All scripts compiled"
