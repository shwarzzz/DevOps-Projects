# Параметры подключения
USER="tomokoki"
HOST="192.168.100.10"
REMOTE_DIR="/usr/local/bin"

scp grep/s21_grep cat/s21_cat  $USER@$HOST:$REMOTE_DIR

if [ $? -eq  0 ]; then
    echo "Success"
    exit 0
else
    echo "Failed"
    exit 1
fi