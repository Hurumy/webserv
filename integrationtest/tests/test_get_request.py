import unittest
import os
from dotenv import load_dotenv
from client.Ipv4Client import Ipv4Client
import requests

load_dotenv()
HOST_NAME = os.getenv('HOST_NAME')
PORT = os.getenv('PORT')
URI_TOP = 'http://' + HOST_NAME + ':' + PORT

class TestGetRequest(unittest.TestCase):

	def test_not_found(self):
		print('\n===========================')
		print('TEST: test simple GET')
		print('===========================\n')

		with open('./content/readme.html') as fs:
			file_text = fs.read()
		# self.client.send('GET /readme.html HTTP/1.1\r\nHost: ' + HOST_NAME + ':' + PORT + '\r\n\r\n')
		# self.client.recv()
		# self.client.close()
		r = requests.get(URI_TOP + '/readme.html')
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, file_text)
		self.assertEqual(r.status_code, 200)

	def test_send_little_by_little(self):
		print('\n===========================')
		print('TEST: test little by little')
		print('===========================\n')
		client = Ipv4Client(HOST_NAME, int(PORT))
		client.send('GET /nos')
		client.send('uchpage HTTP/1.1\r\nHo')
		client.send('st: ' + HOST_NAME + ':' + PORT + '\r\n\r\n')
		client.recv()
		client.close()
		print('Response:\n', client.response_data)
		print('Status: ', client.status)
		self.assertEqual(client.status, '404')
