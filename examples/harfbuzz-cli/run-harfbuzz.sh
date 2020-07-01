set -e

CWD=$(dirname $0)
if [[ $1 == 'windows' ]]; then executable="HarfbuzzCli.exe"; else executable="HarfbuzzCli"; fi

LSAN_OPTIONS=suppressions=lsan.supp $CWD/$executable
