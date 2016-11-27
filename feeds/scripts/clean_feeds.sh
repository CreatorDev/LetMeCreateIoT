#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <lmc_directory>"
    echo "Script for preparing and cleaning header files to simplify patch creation process"
    exit 1
fi

function clear_header_file {
    local FILE=$1

    sed -i.bak -e "/letmecreate\/click\/export/d" $FILE || return 1
    sed -i.bak -e "s/ LETMECREATE_(.*?)_EXPORT / /g" $FILE || return 1

    rm $FILE.bak || return 1
}

function clear_source_file {
    local FILE=$1

    sed -i.bak -e "s;letmecreate/core/gpio_monitor.h;letmecreate/core/interrupts.h;g" $FILE || return 1
    sed -i.bak -r "s/gpio_monitor_add_callback\((.*?),(.*?),( *)(.*?)\)/interrupt_configure(\4)/" $FILE || return 1

    rm $FILE.bak || return 1
}

function check_and_remove_file {
    local FILE=$1

    for pattern in ${REMOVE_CLICKS[@]}; do
        echo "$FILE" | grep -E --quiet "$pattern"
        if [[ $? -eq 0 ]]; then
            rm $FILE || return 2
            return 1
        elif [[ $? -gt 1 ]]; then
            return 2
        fi
    done
}

# List of unsupported clicks to remove
REMOVE_CLICKS=(7seg bargraph)

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
    check_and_remove_file $x
    if [[ $? -eq 1 ]]; then
        echo "Removing.."
        continue
    elif [[ $? -eq 2 ]]; then
        echo "Failed on removing"
        exit 1
    fi

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
    check_and_remove_file $x
    if [[ $? -eq 1 ]]; then
        echo "Removing.."
        continue
    elif [[ $? -eq 2 ]]; then
        echo "Failed on removing"
        exit 1
    fi

    clear_source_file $x
    if [[ $? -ne 0 ]]; then
        echo "Failed to clear source $x"
        exit 1
    fi
done


