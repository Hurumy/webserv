import unittest
from dotenv import load_dotenv
import os
import requests

load_dotenv()
HOST_NAME = os.getenv('HOST_NAME')
PORT = os.getenv('PORT')
HOST_NAME = "localhost"

class TestIndexConfigs(unittest.TestCase):

	def test_simple_get_index_page(self):
		print('\n===========================')
		print('TEST: Simple GET index page')
		print('===========================\n')

		PORT = "8080"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT

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

	def test_having_prior_over_autoindex(self):
		print('\n===========================')
		print('TEST: Having prior over autoindex')
		print('===========================\n')

		PORT = "8081"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT

		with open('./content/readme.html') as fs:
			file_text = fs.read()
		headers = {
			'Host':'sec'
		}
		r = requests.get(URI_TOP + '/', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, file_text)
		self.assertEqual(r.status_code, 200)

	def test_use_index_of_top_scope_instead_of_loaction_one(self):
		print('\n===========================')
		print('TEST: Test use index of top scope instead of loaction one')
		print('===========================\n')

		PORT = "8082"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT

		with open('./content/readme.html') as fs:
			file_text = fs.read()
		headers = {
			'Host':'third'
		}
		r = requests.get(URI_TOP + '/content', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, file_text)
		self.assertEqual(r.status_code, 200)
