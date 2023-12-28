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

	def test_binary_data(self):
		print('\n===========================')
		print('TEST: test GET binary data')
		print('===========================\n')

		with open('./content/wordpress-logo.jpg', 'rb') as fs:
			file_text = fs.read()
		r = requests.get(URI_TOP + '/wordpress-logo.jpg')
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.content, file_text)
		self.assertEqual(r.status_code, 200)

	def test_quoted_string(self):
		print('\n===========================')
		print('TEST: test Quoted string in GET')
		print('===========================\n')

		with open('./content/readme.html') as fs:
			file_text = fs.read()

		headers = {
			'X_TEST_01': 'AA\ A',
			'X_TEST_02': 'AA\,, BBB, CCC'
		}
		r = requests.get(URI_TOP + '/readme.html', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, file_text)
		self.assertEqual(r.status_code, 200)

	def test_quoted_string_nl(self):
		print('\n===========================')
		print('TEST: test Quoted string with a new line in GET')
		print('===========================\n')

		client = Ipv4Client(HOST_NAME, int(PORT))
		client.send('GET /helloworld.txt HTTP/1.1\r\n')
		client.send('Host: ' + HOST_NAME + ':' + PORT + '\r\n')
		client.send("X_TEST: AAA\r\nAAA\r\n")
		client.send("\r\n")
		client.recv()
		client.close()
		print('Response:\n',client.response_data)
		print('Status: ',client.status)
		self.assertEqual(client.status, 200)

	def test_payload_too_large(self):
		print('\n===========================')
		print('TEST: test Payload Too Large to MaxBodySize configuration with GET Method')
		print('===========================\n')

		r = requests.get('http://' + HOST_NAME + ':' + '8083' + '/', data='aaaaaaaaaa')
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 413)
