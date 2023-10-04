import unittest
import os
from dotenv import load_dotenv
from client.Ipv4Client import Ipv4Client

load_dotenv()
HOST_NAME = os.getenv('HOST_NAME')
PORT = os.getenv('PORT')

class TestGetRequest(unittest.TestCase):
	def setUp(self):
		self.client = Ipv4Client(HOST_NAME, int(PORT))

	def test_not_found(self):
		print('\n===========================')
		print('TEST: test not found')
		print('===========================\n')
		self.client.send('GET /nosuchpage HTTP/1.1\r\nHost: ' + HOST_NAME + ':' + PORT + '\r\n\r\n')
		self.client.recv()
		self.client.close()
		print('Response:\n', self.client.response)
		print('Status: ', self.client.status)
		self.assertEqual(self.client.status, '404')

	def test_send_little_by_little(self):
		print('\n===========================')
		print('TEST: test little by little')
		print('===========================\n')
		self.client.send('GET /nos')
		self.client.send('uchpage HTTP/1.1\r\nHo')
		self.client.send('st: ' + HOST_NAME + ':' + PORT + '\r\n\r\n')
		self.client.recv()
		self.client.close()
		print('Response:\n', self.client.response)
		print('Status: ', self.client.status)
		self.assertEqual(self.client.status, '404')
