#!/bin/bash
#
# This script demonstrates some techniques for stylizing a
# table of text with conditional application of SGR codes.
#
# Author: Dylan Doxey <dylan.doxey@gmail.com>
# Date: 01/18/2021
#

TABLE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/../build/bin/table"

function rint()
{
    local from=$1
    local to=$2
    shuf -i ${from}-${to} -n 1
}

function spreadsheet()
{
    echo 'DATE,NAME,PLU,AMOUNT'

    for n in {1..42}
    do
        printf "%02d/%02d/20%02d," $(rint 1 12) $(rint 1 31) $(rint 1 38)
        echo -n "$(tr '0-9' 'a-z' <<< "$RANDOM $RANDOM"),"
        echo -n "$RANDOM,"
        printf "$%d.%02d\n" $(rint 1 1000) $(rint 0 99)
    done
}

function pizzaz()
{
    cat << EOJ
{
    "row": [
        {
            "where": {
                "text": "42"
            },
            "sgr": [5, 45, 97]
        },
        {
            "where": {
                "n": 0
            },
            "sgr": [1, 40, 103],
            "align": "center"
        }
    ],
    "col": [
        {
            "where": {
                "text": "/"
            },
            "sgr": [7, 97, 94]
        },
        {
            "where": {
                "text": "$"
            },
            "sgr": [7, 97, 92]
        }
    ]
}
EOJ
}

function run()
{
    if [[ ! -e "$TABLE" ]]
    then
        echo "$TABLE not built yet" >&2 && return 1
    fi

    spreadsheet | "$TABLE" -s "$(pizzaz)"
}

if [[ $(caller | awk '{print $1}') -eq 0 ]]; then run; fi
