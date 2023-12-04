curl -X POST -T "./content/wordpress-logo.jpg" -H 'Content-type: image/jpeg' --header "Transfer-Encoding: chunked" --header "Connection: Keep-Alive" http://localhost:8081/
ACTUAL=`cat ./content/wordpress-logo.jpg | wc -c`
CMP=`cat ./content/0.jpg | wc -c`
rm -rf ./content/0.jpg
if [ ${ACTUAL} -ne ${CMP} ]; then
	exit 1
fi
if [ "$?" -ne 0 ]; then
	exit 1
fi