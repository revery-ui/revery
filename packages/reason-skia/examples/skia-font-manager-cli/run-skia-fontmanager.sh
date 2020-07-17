set -e

CWD=$(dirname $0)
if [[ $1 == 'windows' ]]; then executable="SkiaFontManagerCli.exe"; else executable="SkiaFontManagerCli"; fi

LSAN_OPTIONS=suppressions=lsan.supp $CWD/$executable
