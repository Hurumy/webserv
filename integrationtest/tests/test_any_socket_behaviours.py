import unittest
import os
from dotenv import load_dotenv
from client.Ipv4Client import Ipv4Client

load_dotenv()
HOST_NAME = os.getenv('HOST_NAME')
PORT = os.getenv('PORT')

class TestAnySocketBehaviours(unittest.TestCase):

	def test_send_but_not_receives(self):
		print('\n===========================')
		print('TEST: test send but not receives')
		print('===========================\n')
		client = Ipv4Client(HOST_NAME, int(PORT))
		client.send('GET / HTTP/1.1\r\n')
		client.close()
		print('Response:\n', client.response_data)
		print('Status: ', client.status)