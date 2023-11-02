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
		r = requests.get(URI_TOP + '/readme.html')
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, file_text)
		self.assertEqual(r.status_code, 200)

	def test_send_little_by_little(self):
		print('\n===========================')
		print('TEST: test little by little')
		print('===========================\n')

		with open('./content/helloworld.txt') as fs:
			file_text = fs.read()
		client = Ipv4Client(HOST_NAME, int(PORT))
		client.send('GET /hel')
		client.send('loworld.txt HTTP/1.1\r\nHo')
		client.send('st: ' + HOST_NAME + ':' + PORT + '\r\n\r\n')
		client.recv()
		client.close()
		print('Response:\n',client.response_data)
		print('Status: ',client.status)
		self.assertEqual(client.response_body, file_text)
		self.assertEqual(client.status, 200)
