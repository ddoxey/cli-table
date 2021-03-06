#!/bin/bash
#
# This script demonstrates some techniques for stylizing a
# table of text with conditional application of SGR codes.
#
# Author: Dylan Doxey <dylan.doxey@gmail.com>
# Date: 01/18/2021
#

NORMAL=0
BOLD=1
LIGHT=2
ITALIC=3
UNDERLINE=4
SLOW_BLINK=5
FAST_BLINK=6
INVERSE=7

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TABLE="${DIR}/../build/bin/table"

function update_style()
{
    local style=$1
    local row=$2
    local col=$3
    local atr=$4
    local bgc=$5
    local fgc=$6
    local c=','

    if [[ ! -s "$style" ]]
    then
        echo -n '{"position":[' > "$style"
        c=""
    elif [[ -z $col ]]
    then
        echo ']}' >> "$style"
        return 0
    fi

    cat << EOJ >> "$style"
${c}
    {
        "where":{
            "row_n":${row},
            "col_n":${col}
        },
        "sgr":[${atr}, ${bgc}, ${fgc}]
    }
EOJ
}

function create_files()
{
    local data="$1"
    local style="$2"

    >$data
    >$style

    local row=0
    local col=0
    local comma=""

    for background in {30..37} {40..47} {90..97} {100..107}
    do
        for foreground in {30..37} {40..47} {90..97} {100..107}
        do
            echo -n "${comma}$BOLD/$background/$foreground" >> $data
            comma=','

            update_style $style $row $col $BOLD $background $foreground

            let col=col+1

            if [[ $col -eq 8 ]]
            then
                let row=row+1
                let col=0
                echo "" >> $data
                comma=""
            fi
        done
    done

    update_style $style
}

function run()
{
    if [[ ! -e "$TABLE" ]]
    then
        echo "$TABLE not built yet" >&2 && return 1
    fi

    local data="${DIR}/sgr-colors.csv"
    local style="${DIR}/sgr-style.json"

    if [[ ! -s "$data" ]] || [[ ! -s "$style" ]]
    then
        create_files "$data" "$style"
    fi

    "$TABLE" "$data" -s "$style"
}


if [[ $(caller | awk '{print $1}') -eq 0 ]]; then run; fi
