#! /bin/bash
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
		cgi_extension py pl cgi;
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

echo 
echo "==== Shutdown webserv ====="
PID=$(ps | grep webserv  | awk '{print $1}')
kill ${PID}
