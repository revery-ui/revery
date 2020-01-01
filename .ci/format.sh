#!/usr/bin/env bash
OS=$1

dune build @fmt --auto-promote

caml_output=$?

if [[ $OS == "windows" ]]
then
    files=$(/usr/bin/find $(pwd) -type f \( -iname \*.c -o -iname \*.h \) -not -path "*_esy/*")
    native_output=$(astyle -n -Q --style=java --s4 $(cygpath -w $files))
elif [[ $OS == "darwin" ]]
then
    native_output=$(find -E . -regex '.*\.(c|h)' -type f -not -path "*_esy/*" -exec astyle -n -Q --style=java --s4 {} \;)
elif [[ $OS == "linux" ]]
then
    native_output=$(find ./ -type f \( -iname \*.c -o -iname \*.h \) -not -path "*_esy/*" -exec astyle -n -Q --style=java --s4 {} \;)
fi

if [[ $native_output ]]
then
    printf "\nFormatted the following native stubs:\n%s" "$native_output"
fi

if [[ $caml_output != 0 ]] || [[ $native_output != "" ]]
then
    exit 1
fi
