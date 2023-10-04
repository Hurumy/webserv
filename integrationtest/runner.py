import os
import sys
import subprocess
from time import  sleep
from unittest import TestLoader
from unittest import TextTestRunner
from dotenv import load_dotenv
from client.Ipv4Client import Ipv4Client

load_dotenv()
WEBSERV = os.getenv('WEBSERV')
CONFFILE = os.getenv('CONFFILE')

def main(path):
	loader = TestLoader()
	test = loader.discover(path)
	runner = TextTestRunner()
	runner.run(test)

if __name__ == '__main__':
	if len(sys.argv) != 2:
		print('usage: %s path' % sys.argv[0])
		sys.exit(1)
	print('Run webserv')
	proc = subprocess.Popen([WEBSERV, CONFFILE])
	sleep(1)
	print('Run Test')
	main(sys.argv[1])
	print('Terminate webserv')
	proc.terminate()
