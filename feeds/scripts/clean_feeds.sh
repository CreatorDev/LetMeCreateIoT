#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <lmc_directory>"
    echo "Script for preparing and cleaning header files to simplify patch creation process"
    exit 1
fi

function clear_header_file {
    local FILE=$1

    sed -i.bak -e "/letmecreate\/click\/export/d" $FILE || exit 1
    sed -i.bak -e "s/ LETMECREATE_(.*?)_EXPORT / /g" $FILE || exit 1

    rm $FILE.bak || exit 1
}

function clear_source_file {
    local FILE=$1

    sed -i.bak -e "s;letmecreate/core/gpio_monitor.h;letmecreate/core/interrupts.h;g" $FILE || exit 1
    sed -i.bak -r "s/gpio_monitor_add_callback\((.*?),(.*?),( *)(.*?)\)/interrupt_configure(\4)/" $FILE || exit 1

    rm $FILE.bak || exit 1
}


HEADER_DIRECTORY=$(readlink -f $1/include/letmecreate/click)
SOURCE_DIRECTORY=$(readlink -f $1/src/click)


echo "Moving to $HEADER_DIRECTORY"
cd $HEADER_DIRECTORY
if [[ $? -ne 0 ]]; then
    echo "Could not find the header directory"
    exit 1
fi

for x in *.h; do
    echo "Clearing $x"
    clear_header_file $x
    if [[ $? -ne 0 ]]; then
        echo "Failed to clear header $x"
        exit 1
    fi
done

echo "Moving to $SOURCE_DIRECTORY"
cd $SOURCE_DIRECTORY
if [[ $? -ne 0 ]]; then
    echo "Could not find the source directory"
    exit 1
fi

for x in *.c; do
    echo "Clearing $x"
    clear_source_file $x
    if [[ $? -ne 0 ]]; then
        echo "Failed to clear source $x"
        exit 1
    fi
done


