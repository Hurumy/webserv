import unittest
from dotenv import load_dotenv
import os
import requests

load_dotenv()
HOST_NAME = "localhost"

class TestRewriteConfigs(unittest.TestCase):

	def test_simple_get_rewrite_page(self):
		print('\n===========================')
		print('TEST: Simple GET rewrite page')
		print('===========================\n')

		PORT = "8080"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT

		with open('./content/a/index.html') as fs:
			file_text = fs.read()
		headers = {
			'Host':'def'
		}
		r = requests.get(URI_TOP + '/content/', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, file_text)
		self.assertEqual(r.status_code, 200)
