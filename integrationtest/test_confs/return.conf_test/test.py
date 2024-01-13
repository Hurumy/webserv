import unittest
from dotenv import load_dotenv
import os
import requests

load_dotenv()
HOST_NAME = os.getenv('HOST_NAME')
PORT = os.getenv('PORT')
HOST_NAME = "localhost"
PORT = "8080"
URI_TOP = 'http://' + HOST_NAME + ':' + PORT

class TestReturnConfigs(unittest.TestCase):

	def test_simple_get_index_page(self):
		print('\n===========================')
		print('TEST: Simple GET redirect page')
		print('===========================\n')

		with open('./content/readme.html') as fs:
			file_text = fs.read()
		headers = {
			'Host':'def'
		}
		r = requests.get(URI_TOP + '/', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, file_text)
		self.assertEqual(r.status_code, 200)
