import unittest
from dotenv import load_dotenv
import os
import requests

load_dotenv()
HOST_NAME = os.getenv('HOST_NAME')
PORT = os.getenv('PORT')
HOST_NAME = "localhost"

class TestLClientMaxBodySizeConfigs(unittest.TestCase):

	def test_post_with_max_body_size(self):
		print('\n===========================')
		print('TEST: POST with max body size')
		print('===========================\n')

		PORT = "8080"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT

		data = 'aaaaa'
		headers = {
			'Host':'def',
			'Content-Type':'text/plain'
		}
		r = requests.get(URI_TOP + '/content/a/', data=data, headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 201)
		if os.path.exists('./content/a/0.txt'): os.remove('./content/a/0.txt')

	def test_post_with_over_max_body_size(self):
		print('\n===========================')
		print('TEST: POST with max body size')
		print('===========================\n')

		PORT = "8081"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT

		data = 'aaaaaa'
		headers = {
			'Host':'def',
			'Content-Type':'text/plain'
		}
		r = requests.get(URI_TOP + '/content/a/', data=data, headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 413)
		if os.path.exists('./content/a/0.txt'): os.remove('./content/a/0.txt')

	def test_having_prior_over_top_scope(self):
		print('\n===========================')
		print('TEST: Having prior over top scope')
		print('===========================\n')

		PORT = "8081"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT

		data = 'aaaaa'
		headers = {
			'Host':'sec',
			'Content-Type':'text/plain'
		}
		r = requests.get(URI_TOP + '/content/a/', data=data, headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 201)
		if os.path.exists('./content/a/0.txt'): os.remove('./content/a/0.txt')
