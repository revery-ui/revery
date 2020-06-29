set -e

CWD=$(dirname $0)
LSAN_OPTIONS=suppressions=lsan.supp $CWD/HarfbuzzCli.exe
