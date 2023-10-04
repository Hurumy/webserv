#!/bin/bash

set -e

echo "${0}: install telnet."
apt install -y telnet
status=$?
if [ $status -ne 0 ]; then
  echo "Failed to install telnet: $status"
  exit $status
fi

echo "${0}: install siege."
apt install -y siege
status=$?
if [ $status -ne 0 ]; then
  echo "Failed to install siege: $status"
  exit $status
fi

echo "${0}: install robotframework."
pip install robotframework
status=$?
if [ $status -ne 0 ]; then
  echo "Failed to install robotframework: $status"
  exit $status
fi

echo "${0}: install robotframework-requests."
pip install robotframework-requests
status=$?
if [ $status -ne 0 ]; then
  echo "Failed to install robotframework-requests: $status"
  exit $status
fi

echo "${0}: install python-dotenv."
pip install python-dotenv
status=$?
if [ $status -ne 0 ]; then
  echo "Failed to install python-dotenv: $status"
  exit $status
fi

echo "${0}: compile sources."
make re
status=$?
if [ $status -ne 0 ]; then
  echo "Failed to compile: $status"
  exit $status
fi

sleep infinity
