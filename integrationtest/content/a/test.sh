#!/bin/bash

FILE=$(test.py)
LENGTH=$(wc -l $FILE | awk '{print $1}')

echo "Content-type: text/html"
echo "Content-Length: $LENGTH"
echo
cat $FILE
