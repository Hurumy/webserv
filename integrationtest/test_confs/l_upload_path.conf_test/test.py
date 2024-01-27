import unittest
from dotenv import load_dotenv
import os
import requests

load_dotenv()
HOST_NAME = os.getenv('HOST_NAME')
PORT = os.getenv('PORT')
HOST_NAME = "localhost"

class TestRootConfigs(unittest.TestCase):

	def test_simple_post_to_upload_path(self):
		print('\n===========================')
		print('TEST: Test simple POST content under root')
		print('===========================\n')

		PORT = "8080"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT

        data = '1234567890'
		headers = {
			'Host':'def'
			'Content-Type':'text/plain'
		}
		r = requests.get(URI_TOP + '/', data=data, headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
        
        with open('./content/0.txt') as fs:
            file_text = fs.read()
		self.assertEqual(file_text, data)
		self.assertEqual(r.status_code, 201)
        if os.path.exists('./content/0.txt'): os.remove('./content/0.txt')

	def test_having_prior_over_top_scope(self):
		print('\n===========================')
		print('TEST: Test having prior over top scope')
		print('===========================\n')

		PORT = "8081"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT

        data = '1234567890'
		headers = {
			'Host':'def'
			'Content-Type':'text/plain'
		}
		r = requests.get(URI_TOP + '/', data=data, headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
        
        with open('./content/0.txt') as fs:
            file_text = fs.read()
		self.assertEqual(file_text, data)
		self.assertEqual(r.status_code, 201)
        if os.path.exists('./content/0.txt'): os.remove('./content/0.txt')
