#! /bin/bash
set -o allexport
source .env set

echo 
echo "==== Compile webserv ====="
if [ "$1" = "remake" ]; then
	make fclean -C ${PROJECT_ROOT}
fi
make -C ${PROJECT_ROOT}

if [ "$?" -ne 0 ]; then
	exit 1
fi

echo 
echo "==== Start webserv ====="
${WEBSERV} ${CONFFILE} &

echo 
echo "==== Run Integration tests ====="
python3 runner.py tests

echo 
echo "==== Run shell test ====="
bash tests/test.sh

echo 
echo "==== Shutdown webserv ====="
PID=$(ps | grep webserv  | awk '{print $1}')
kill ${PID}
