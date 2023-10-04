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
		self.client.send('GET /nosuchpage HTTP/1.1\r\nHost: ' + HOST_NAME + ':' + PORT + '\r\n\r\n')
		self.client.recv()
		self.client.close()
		print(self.client.response)
		self.assertEqual(self.client.status, '404')
