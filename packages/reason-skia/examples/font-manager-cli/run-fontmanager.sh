set -e

CWD=$(dirname $0)
if [[ $1 == 'windows' ]]; then executable="FontManagerCli.exe"; else executable="FontManagerCli"; fi

LSAN_OPTIONS=suppressions=../reason-harfbuzz/lsan.supp $CWD/$executable
