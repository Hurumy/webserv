#! /bin/bash

ESC=$(printf '\033') RESET="${ESC}[0m"

BOLD="${ESC}[1m"        FAINT="${ESC}[2m"       ITALIC="${ESC}[3m"
UNDERLINE="${ESC}[4m"   BLINK="${ESC}[5m"       FAST_BLINK="${ESC}[6m"
REVERSE="${ESC}[7m"     CONCEAL="${ESC}[8m"     STRIKE="${ESC}[9m"

GOTHIC="${ESC}[20m"     DOUBLE_UNDERLINE="${ESC}[21m" NORMAL="${ESC}[22m"
NO_ITALIC="${ESC}[23m"  NO_UNDERLINE="${ESC}[24m"     NO_BLINK="${ESC}[25m"
NO_REVERSE="${ESC}[27m" NO_CONCEAL="${ESC}[28m"       NO_STRIKE="${ESC}[29m"

BLACK="${ESC}[30m"      GREEN="${ESC}[31m"        GREEN="${ESC}[32m"
YELLOW="${ESC}[33m"     BLUE="${ESC}[34m"       MAGENTA="${ESC}[35m"
CYAN="${ESC}[36m"       WHITE="${ESC}[37m"      DEFAULT="${ESC}[39m"

BG_BLACK="${ESC}[40m"   BG_GREEN="${ESC}[41m"     BG_GREEN="${ESC}[42m"
BG_YELLOW="${ESC}[43m"  BG_BLUE="${ESC}[44m"    BG_MAGENTA="${ESC}[45m"
BG_CYAN="${ESC}[46m"    BG_WHITE="${ESC}[47m"   BG_DEFAULT="${ESC}[49m"

set -o allexport

echo "\
HOST_NAME='localhost'
PORT='8081'
PROJECT_ROOT='../'
CONFFILE=\"\${PROJECT_ROOT}integrationtest/conf/test.conf\"
WEBSERV=\"\${PROJECT_ROOT}webserv\"
" > .env

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
echo "==== Create conf file ====="
echo "server {
		listen 			8081;
		server_name  localhost;
		root		 /content;
 
		index readme.html;
		autoindex on;
		upload_path	$PWD/content;
		allowedMethods	GET POST DELETE;
		cgi_extension py pl;
		client_max_body_size 5m;
}

server {
		listen 			8082;
		server_name  localhost;
		root		 /content;
 
		index readme.html;
		autoindex on;
		allowedMethods	GET POST DELETE;
		cgi_extension py pl;
		client_max_body_size 5m;
}

server {
		listen 			8083;
		server_name  localhost;
		root		 /content;
 
		index readme.html;
		autoindex on;
		allowedMethods	GET POST DELETE;
		upload_path	$PWD/content;
		cgi_extension py pl;
		client_max_body_size 5;
}
" > ${CONFFILE}

echo
echo "==== Reinstall dotenv ====="
pip install -U python-dotenv
if [ "$?" -ne 0 ]; then
	exit 1
fi

echo
echo "==== Allow webserv to execute CGI ====="
chmod -R 775 content/cgi_bin/
if [ "$?" -ne 0 ]; then
	exit 1
fi

echo
echo "==== Start webserv ====="
${WEBSERV} ${CONFFILE} &
if [ "$?" -ne 0 ]; then
	PID=$(ps | grep webserv  | awk '{print $1}')
	kill ${PID}
	exit 1
fi


<<COMMENTOUT
echo 
echo "==== Run Integration tests ====="
python runner.py tests
if [ "$?" -ne 0 ]; then
	PID=$(ps | grep webserv  | awk '{print $1}')
	kill ${PID}
	exit 1
fi

echo 
echo "==== Run shell test ====="
bash tests/test.sh
if [ "$?" -ne 0 ]; then
	PID=$(ps | grep webserv  | awk '{print $1}')
	kill ${PID}
	exit 1
fi
COMMENTOUT

echo 
echo "==== Shutdown webserv ====="
PID=$(ps | grep webserv  | awk '{print $1}')
kill ${PID}


# Configテスト用ループ
for FILE in `ls ./test_confs/*.conf`
do

TESTDIR=${FILE}_test

echo
echo "${ESC}${GREEN}${ESC} Starting test with $(echo ${FILE} | xargs basename) ${ESC}${RESET}${ESC}"
echo
echo "${ESC}${GREEN}${ESC} Testdir Name: $(echo ${TESTDIR}) ${ESC}${RESET}${ESC}"


echo
echo "${ESC}${GREEN}${ESC} = Start webserv =${ESC}${RESET}${ESC}"
${WEBSERV} ${FILE} &
if [ "$?" -ne 0 ]; then
	PID=$(ps | grep webserv  | awk '{print $1}')
	kill ${PID}
	exit 1
fi

echo 
echo "${ESC}${GREEN}${ESC} = Run Integration tests = ${ESC}${RESET}${ESC}"
echo
python runner.py ${TESTDIR}
echo
if [ "$?" -ne 0 ]; then
	PID=$(ps | grep webserv  | awk '{print $1}')
	kill ${PID}
	exit 1
fi

echo 
echo "${ESC}${GREEN}${ESC} = Shutdown webserv =${ESC}${RESET}${ESC}"
echo
PID=$(ps | grep webserv  | awk '{print $1}')
kill ${PID} > /dev/null 2>&1

done

