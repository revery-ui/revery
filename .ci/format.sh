#!/usr/bin/env bash
OS=$1

dune build @fmt --auto-promote

caml_output=$?

if [[ $OS == "windows" ]]
then
    native_files=$(/usr/bin/find $(pwd) -type f \( -iname \*.c -o -iname \*.h -o -iname \*.cpp \) -not -path "*_esy/*")
    native_output=$(astyle -n -Q --style=java --s4 $(cygpath -w $native_files))

    dune_files=$(/usr/bin/find $(pwd) -type f \( -iname .*\dune \) -not -path "*_esy/*")
elif [[ $OS == "darwin" ]]
then
    native_output=$(find -E . -regex '.*\.(c|h|cpp)' -type f -not -path "*_esy/*" -exec astyle -n -Q --style=java --s4 {} \;)
    
    dune_files=$(find -E . -regex '.*\dune' -type f -not -path "*_esy/*" -not -path "./.git/*")
elif [[ $OS == "linux" ]]
then
    native_output=$(find ./ -type f \( -iname \*.c -o -iname \*.h -o -iname \*.cpp \) -not -path "*_esy/*" -exec astyle -n -Q --style=java --s4 {} \;)

    dune_files=$(find ./ -type f \( -iname .*\dune \) -not -path "*_esy/*")
fi

dune_output=0
for dune_file in $dune_files
do
    pre=$(cat $dune_file)
    post=$(dune format-dune-file $dune_file)
    diff <(echo "$pre") <(echo "$post")
    diff_output=$?
    if [[ $diff_output != 0 ]]
    then
        dune_output=1
        echo "$post" > $dune_file
    fi
done

if [[ $native_output ]]
then
    printf "\nFormatted the following native stubs:\n%s\n" "$native_output"
fi

if [[ $caml_output != 0 ]] || [[ $native_output != "" ]] || [[ $dune_output != 0 ]]
then
    exit 1
fi
