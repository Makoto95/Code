#!/bin/bash

function usage {
    echo "Usage: $0 filename"
    echo "ファイル名は01A.tarのように, 班名をつけること."
    echo "tarにする方法は実験ページを参照."
}

# Argument Validation
if [ "$#" -ne "1" ]; then
    usage
    exit 1
fi
if [ ! -f "$1" ]; then
    echo "'$1' is not found"
    echo "--"
    usage
    exit 1
fi
if [[ ! "$1" =~ \.tar$ ]]; then
    echo "'$1' is not a TAR format"
    echo "--"
    usage
    exit 1
fi

# Variants
filepath="$1"
filename=$(basename $1 .tar)
dirpath=$(dirname $1)
aftername="${filename}_`date +%Y%m%d_%H%M%S`.tar"

# Filename Validation
if [[ ! "${filename}" =~ ^[0-9][0-9][AB](-final)?$ ]]; then
    echo "'${filename}.tar' is malformed."
    echo "Filename must be your group number like '01A.tar'."
    exit 1
fi

# Send
scp -oPubkeyAuthentication=no -oPasswordAuthentication=yes "$filepath" "i1i2i3-submit@i1i2i3.eidos.ic.i.u-tokyo.ac.jp:$aftername" > /dev/null
if [ $? -eq 0 ]; then
    echo "Success!"
    echo "Submitted as: ${aftername}"
    exit 0
else
    echo "Submittion is failed."
    echo "Please retry. (Network is disconnected?)"
fi
