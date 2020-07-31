set -e

CWD=$(dirname $0)
if [[ $1 == 'windows' ]]; then executable="Cli.exe"; else executable="Cli"; fi

LSAN_OPTIONS=suppressions=lsan.supp $CWD/$executable
