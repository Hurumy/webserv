import unittest
from dotenv import load_dotenv
import os
import requests

load_dotenv()
HOST_NAME = "localhost"
PORT = "8080"
URI_TOP = 'http://' + HOST_NAME + ':' + PORT

class TestClientMaxBodySizeConfigs(unittest.TestCase):

	def test_post_with_max_body_size(self):
		print('\n===========================')
		print('TEST: POST with max body size')
		print('===========================\n')

		data = 'aaaaa'
		headers = {
			'Host':'def',
			'Content-Type':'text/plain'
		}
		r = requests.get(URI_TOP + '/content/', data=data, headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 201)
		if os.path.exists('./content/0.txt'): os.remove('./content/0.txt')

	def test_post_with_over_max_body_size(self):
		print('\n===========================')
		print('TEST: POST with max body size')
		print('===========================\n')

		data = 'aaaaaa'
		headers = {
			'Host':'def',
			'Content-Type':'text/plain'
		}
		r = requests.get(URI_TOP + '/content/', data=data, headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 413)
		if os.path.exists('./content/0.txt'): os.remove('./content/0.txt')
