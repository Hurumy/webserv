import unittest
from dotenv import load_dotenv
import os
import requests

load_dotenv()
HOST_NAME = os.getenv('HOST_NAME')
PORT = os.getenv('PORT')
URI_TOP = 'http://' + HOST_NAME + ':' + PORT

class TestCGIRequest(unittest.TestCase):

	def test_simple_document_response(self):
		print('\n===========================')
		print('TEST: Simple Document response')
		print('===========================\n')

		r = requests.get(URI_TOP + '/cgi_bin/test.py')
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 200)

	def test_simple_client_redirect_response(self):
		print('\n===========================')
		print('TEST: Simple Client redirect response')
		print('===========================\n')

		r = requests.get(URI_TOP + '/cgi_bin/test.py')
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 200)

	def test_too_large_request(self):
		print('\n===========================')
		print('TEST: Too large request body')
		print('===========================\n')

		with open('./data/twice_large_buffersize') as fs:
			file_text = fs.read()
		r = requests.post(URI_TOP + '/cgi_bin/test.py', data=file_text)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 200)

	def test_invalid_local_redirect_response_extra_lines(self):
		print('\n===========================')
		print('TEST: Invalid local redirect response (Extra lines)')
		print('===========================\n')

		r = requests.get(URI_TOP + '/cgi_bin/invalid_local_redirect_response_extra_lines.pl')
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 500)

	def test_infinite_loop_local_redirect(self):
		print('\n===========================')
		print('TEST: Infinite loop local redirect response')
		print('===========================\n')

		r = requests.get(URI_TOP + '/cgi_bin/infinite_loop_local_redirect_response.py')
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 500)
