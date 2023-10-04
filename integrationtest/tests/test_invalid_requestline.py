import unittest
import os
from dotenv import load_dotenv
from client.Ipv4Client import Ipv4Client

load_dotenv()
HOST_NAME = os.getenv('HOST_NAME')
PORT = os.getenv('PORT')

class TestInvalidRequestLine(unittest.TestCase):
	def setUp(self):
		self.client = Ipv4Client(HOST_NAME, int(PORT))

	def test_extra_word(self):
		print('\n===========================')
		print('TEST: test extra word')
		print('===========================\n')
		self.client.send('GET /nosuchpage HTTP/1.1 thisisextra\r\n')
		self.client.recv()
		self.client.close()
		print('Response:\n', self.client.response)
		print('Status: ', self.client.status)
