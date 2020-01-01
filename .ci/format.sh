#!/bin/bash
set -e
OS=$1

dune build @fmt --auto-promote

if [ $OS == "windows" ]
then
    # Prevents "Environment too big" error on Windows
    files=$(/usr/bin/find ./ -type f \( -iname \*.c -o -iname \*.h \) -not -path "*_esy/*")
    native_output=$(astyle -n -Q --style=java --s4 $files)
elif [ $OS == "darwin" ]
then
    native_output=$(find -E . -regex '.*\.(c|h)' -type f -not -path "*_esy/*" -exec astyle -n -Q --style=java --s4 {} \;)
elif [ $OS == "linux" ]
then
    native_output=$(find ./ -type f \( -iname \*.c -o -iname \*.h \) -not -path "*_esy/*" -exec astyle -n -Q --style=java --s4 {} \;)
fi

if [ $native_output ];
then
    printf "Formatted the following native stubs:\n$native_output"; exit 1
fi
