import unittest
from dotenv import load_dotenv
import os
import requests

load_dotenv()
HOST_NAME = "localhost"

class TestLReturnConfigs(unittest.TestCase):

	def test_simple_get_redirect_page(self):
		print('\n===========================')
		print('TEST: Simple GET redirect page')
		print('===========================\n')

		PORT = "8080"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT

		with open('./content/readme.html') as fs:
			file_text = fs.read()
		headers = {
			'Host':'def'
		}
		r = requests.get(URI_TOP + '/content/a/', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, file_text)
		self.assertEqual(r.status_code, 200)

	def test_having_prior_over_top_scope(self):
		print('\n===========================')
		print('TEST: Test having prior over top scope')
		print('===========================\n')

		PORT = "8081"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT

		with open('./content/readme.html') as fs:
			file_text = fs.read()
		headers = {
			'Host':'sec'
		}
		r = requests.get(URI_TOP + '/content/a/', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, file_text)
		self.assertEqual(r.status_code, 200)
