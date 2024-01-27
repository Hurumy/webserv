import unittest
from dotenv import load_dotenv
import os
import requests

load_dotenv()
HOST_NAME = os.getenv('HOST_NAME')
PORT = os.getenv('PORT')
HOST_NAME = "localhost"

class TestRootConfigs(unittest.TestCase):

	def test_simple_get_content_under_root(self):
		print('\n===========================')
		print('TEST: Test simple get content under root')
		print('===========================\n')

		PORT = "8080"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT

		with open('./content/readme.html') as fs:
			file_text = fs.read()
		headers = {
			'Host':'def'
		}
		r = requests.get(URI_TOP + '/readme.html', headers=headers)
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

		with open('./content/a/index.html') as fs:
			file_text = fs.read()
		headers = {
			'Host':'def'
		}
		r = requests.get(URI_TOP + '/index.html', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, file_text)
		self.assertEqual(r.status_code, 200)
