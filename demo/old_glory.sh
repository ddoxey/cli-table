#!/bin/bash
#
# This script demonstrates some techniques for stylizing a
# table of text with conditional application of SGR codes.
#
# Author: Dylan Doxey <dylan.doxey@gmail.com>
# Date: 01/18/2021
#

TABLE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/../build/bin/table"

function flag()
{
cat << EOF | sed -e 's/\(.\)/\1,/g' -e 's/,$//'
* * *rrrrrr
 * * wwwwww
* * *rrrrrr
 * * wwwwww
* * *rrrrrr
 * * wwwwww
* * *rrrrrr
wwwwwwwwwww
rrrrrrrrrrr
wwwwwwwwwww
rrrrrrrrrrr
wwwwwwwwwww
rrrrrrrrrrr
EOF
}

function red_white_blue()
{
cat << EOJ
{
    "col": [
        {
            "where": {
                "text": "*"
            },
            "sgr": [1, 97, 104]
        },
        {
            "where": {
                "text": " "
            },
            "sgr": [0, 94, 104]
        },
        {
            "where": {
                "text": "r"
            },
            "sgr": [2, 91, 101]
        },
        {
            "where": {
                "text": "w"
            },
            "sgr": [2, 97, 97]
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

    flag | "$TABLE" -s "$(red_white_blue)"
}

if [[ $(caller | awk '{print $1}') -eq 0 ]]; then run; fi
