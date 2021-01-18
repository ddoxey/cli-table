#!/bin/bash

TABLE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/../build/bin/table"

function flag()
{
    echo '*+*+*======'
    echo '+*+*+wwwwww'
    echo '*+*+*======'
    echo '+*+*+wwwwww'
    echo '*+*+*======'
    echo 'wwwwwwwwwww'
    echo '==========='
    echo 'wwwwwwwwwww'
    echo '==========='
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
            "sgr": [1, 104, 97]
        },
        {
            "where": {
                "text": "+"
            },
            "sgr": [7, 94, 104]
        },
        {
            "where": {
                "text": "="
            },
            "sgr": [7, 91, 101]
        },
        {
            "where": {
                "text": "w"
            },
            "sgr": [2, 30, 97]
        }
    ]
}
EOJ
}

function run()
{
    if [[ ! -e $TABLE ]]
    then
        echo "$TABLE not built yet" >&2 && return 1
    fi

    local style=$(mktemp)

    red_white_blue > $style

    flag | sed -e 's/\(.\)/\1,/g' -e 's/,$//' | $TABLE -s $style

    rm -f $style
}

if [[ $(caller | awk '{print $1}') -eq 0 ]]; then run; fi
