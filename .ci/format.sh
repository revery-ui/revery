set -e
OS=$(uname)

if [[ $OS =~ "CYWGIN" ]];
then
    find . -name "*.re" -type f -exec refmt --in-place {} \;
else
    dune build @fmt --auto-promote
fi

native_output=$(find -E . -regex '.*\.(c|h)' -type f -not -path "*_esy/*" -exec astyle -n -Q --style=java --s4 {} \;)

if [[ $native_output ]];
then
    printf "Formatted the following native stubs:\n$native_output"; exit 1
fi