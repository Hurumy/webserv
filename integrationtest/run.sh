#! /bin/bash
set -o allexport
source .env set

echo "==== Compile webserv ====="
make -C ${PROJECT_ROOT}

echo "==== Start webserv ====="
${WEBSERV} ${CONFFILE} &

echo "==== Run Integration tests ====="
python3 runnner.py
