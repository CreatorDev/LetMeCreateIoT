#!/bin/bash

BASE_DIR=$(dirname $(readlink -f $0))/..

$BASE_DIR/install.sh -u

if [[ -L $BASE_DIR/contiki ]]; then
    echo "Uninstall test failed"
    exit 1
fi

echo "Uninstall test passed"
exit 0

