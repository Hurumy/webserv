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

class TestLIndexConfigs(unittest.TestCase):

	def test_simple_get_l_index_page(self):
		print('\n===========================')
		print('TEST: Simple GET l_index page')
		print('===========================\n')

		with open('./content/a/index.html') as fs:
			file_text = fs.read()
		headers = {
			'Host':'def'
		}
		r = requests.get(URI_TOP + '/content/a/', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, file_text)
		self.assertEqual(r.status_code, 200)

	def test_having_prior_over_autoindex_l_index_ver(self):
		print('\n===========================')
		print('TEST: Having prior over autoindex l_index ver')
		print('===========================\n')

		with open('./content/a/index.html') as fs:
			file_text = fs.read()
		headers = {
			'Host':'sec'
		}
		r = requests.get(URI_TOP + '/content/a/', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, file_text)
		self.assertEqual(r.status_code, 200)

	def test_having_prioer_over_top_scope_index(self):
		print('\n===========================')
		print('TEST: Test having prioer over top scope index')
		print('===========================\n')

		with open('./content/a/index.html') as fs:
			file_text = fs.read()
		headers = {
			'Host':'third'
		}
		r = requests.get(URI_TOP + '/content/a/', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, file_text)
		self.assertEqual(r.status_code, 200)