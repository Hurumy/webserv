import unittest
from dotenv import load_dotenv
import os
import requests

load_dotenv()
HOST_NAME = os.getenv('HOST_NAME')
PORT = os.getenv('PORT')
URI_TOP = 'http://' + HOST_NAME + ':' + PORT

class TestCGIRequest(unittest.TestCase):

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
