#!/bin/bash

TABLE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/../build/bin/table"

function spreadsheet()
{
    echo 'DATE,NAME,PLU,AMOUNT'

    for n in {1..42}
    do
        echo -n "${n}/1/2021,"
        echo -n "$(tr '0-9' 'a-z' <<< "$RANDOM"),"
        echo -n "$RANDOM,"
        sed 's/\(...\)\(..\)/\1.\2/' <<< "\$$RANDOM"
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
            "sgr": [7, 35, 30],
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

    local style=$(mktemp)

    pizzaz > "$style"

    spreadsheet | "$TABLE" -s "$style"

    rm -f "$style"
}

if [[ $(caller | awk '{print $1}') -eq 0 ]]; then run; fi
