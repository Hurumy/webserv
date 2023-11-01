import unittest
import os
from dotenv import load_dotenv
from client.Ipv4Client import Ipv4Client

load_dotenv()
HOST_NAME = os.getenv('HOST_NAME')
PORT = os.getenv('PORT')

class TestGetRequest(unittest.TestCase):

	maxDiff = None

	def setUp(self):
		self.client = Ipv4Client(HOST_NAME, int(PORT))

	def test_not_found(self):
		print('\n===========================')
		print('TEST: test simple GET')
		print('===========================\n')

		fs = open('./content/readme.html')
		file_text = fs.read()
		fs.close()
		self.client.send('GET /readme.html HTTP/1.1\r\nHost: ' + HOST_NAME + ':' + PORT + '\r\n\r\n')
		self.client.recv()
		self.client.close()
		print('Response:\n', self.client.response_data)
		print('Status: ', self.client.status)
		self.assertEqual(self.client.status, '200')
		self.assertEqual(self.client.response_body, file_text)

	def test_send_little_by_little(self):
		print('\n===========================')
		print('TEST: test little by little')
		print('===========================\n')
		self.client.send('GET /nos')
		self.client.send('uchpage HTTP/1.1\r\nHo')
		self.client.send('st: ' + HOST_NAME + ':' + PORT + '\r\n\r\n')
		self.client.recv()
		self.client.close()
		print('Response:\n', self.client.response_data)
		print('Status: ', self.client.status)
		self.assertEqual(self.client.status, '404')
