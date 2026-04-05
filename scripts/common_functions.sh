# common_functions.sh
# Библиотека общих функций. Не для прямого запуска.

# Цвета для терминала
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

function get_answer {
    if [ "$AUTO_YES" = true ]; then
        echo "y (авто)"
        return 0
    fi

    while true; do
        read -n 1 answer
        echo
        case "$answer" in
            [Yy]* ) answer="y"; return 0 ;;
            [Nn]* ) answer="n"; return 1 ;;
            *) echo -ne "${RED}Ошибка ввода, введите [y/n]: ${NC}" ;;
        esac
    done
}