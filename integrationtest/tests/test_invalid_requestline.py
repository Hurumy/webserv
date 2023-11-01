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
		print('Response:\n', self.client.response_data)
		print('Status: ', self.client.status)

	def test_insufficient_word(self):
		print('\n===========================')
		print('TEST: test insufficient word')
		print('===========================\n')
		self.client.send('GET / \r\n')
		self.client.recv()
		self.client.close()
		print('Response:\n', self.client.response_data)
		print('Status: ', self.client.status)

	def test_invalid_location01(self):
		print('\n===========================')
		print('TEST: test invalid location 01')
		print('===========================\n')
		self.client.send('GET ../ HTTP/1.1\r\n')
		self.client.recv()
		self.client.close()
		print('Response:\n', self.client.response_data)
		print('Status: ', self.client.status)

	def test_invalid_location02(self):
		print('\n===========================')
		print('TEST: test invalid location 02')
		print('===========================\n')
		self.client.send('GET ./ HTTP/1.1\r\n')
		self.client.recv()
		self.client.close()
		print('Response:\n', self.client.response_data)
		print('Status: ', self.client.status)
		print('Status: ', self.client.status)

	def test_invalid_location03(self):
		print('\n===========================')
		print('TEST: test invalid location 03')
		print('===========================\n')
		self.client.send('GET /you/../../ HTTP/1.1\r\n')
		self.client.recv()
		self.client.close()
		print('Response:\n', self.client.response_data)
		print('Status: ', self.client.status)

	def test_invalid_location04(self):
		print('\n===========================')
		print('TEST: test invalid location 04')
		print('===========================\n')
		self.client.send('GET /./../ HTTP/1.1\r\n')
		self.client.recv()
		self.client.close()
		print('Response:\n', self.client.response_data)
		print('Status: ', self.client.status)

	def test_invalid_version(self):
		print('\n===========================')
		print('TEST: test invalid version')
		print('===========================\n')
		self.client.send('GET / HTTP/1.11\r\n')
		self.client.recv()
		self.client.close()
		print('Response:\n', self.client.response_data)
		print('Status: ', self.client.status)
