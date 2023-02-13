#!/bin/bash
#
# Use this script to build cli-table on a Ubuntu system.
#
# ./build.sh     # build
#
# ./build.sh -p  # make clean & purge cmake artfacts
#
CMAKE="$(which cmake)"
MAKE="$(which make)"

function build()
{
    cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

    if [[ -d build ]]; then rm -rf build; fi
    mkdir -p build || return 1
    cd build

    $CMAKE .. || return 1

    if [[ "$1" == "-p" ]]
    then
        $MAKE clean || return 1

        rm bin/* 2>/dev/null
        rm lib/* 2>/dev/null
        rm -rf Makefile CMakeCache.txt CMakeFiles cmake_install.cmake

        echo "-- Build files have been purged from: $(pwd)"

        return 0
    fi

    $MAKE || return 1
}

function main()
{
    if [[ -z $MAKE ]]
    then
        echo "make is not installed" >&2 && return 1
    fi

    if [[ -z $CMAKE ]]
    then
        echo "cmake not installed" >&2 && return 1
    fi

    local json_hpp=$(find /usr -type f -name 'json.hpp' 2>/dev/null | grep 'nlohmann')

    if [[ -z $json_hpp ]]
    then
        echo "nlohmann/json.hpp not found" >&2 && return 1
    fi

    local gtest_h=$(find /usr -type f -name 'gtest.h' 2>/dev/null | grep 'gtest.gtest')

    if [[ -z $gtest_h ]]
    then
        echo "gtest/gtest.h not found" >&2 && return 1
    fi

    build "$@"
}

if [[ $(caller | awk '{print $1}') -eq 0 ]]; then main "$@"; fi
