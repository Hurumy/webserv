import unittest
from dotenv import load_dotenv
import os
import requests

load_dotenv()
#HOST_NAME = os.getenv('HOST_NAME')
#PORT = os.getenv('PORT')
# HOST_NAME = "localhost"
# PORT = "25565"
# URI_TOP = 'http://' + HOST_NAME + ':' + PORT

class TestAutoIndexConfigs(unittest.TestCase):

	def test_autoindex_on(self):
		print('\n===========================')
		print('TEST: autoindex Configs on')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25573"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'def'
		}
		# try to get
		r = requests.get(URI_TOP + '/content/a/', headers=headers)
		#print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 200)

	def test_autoindex_off(self):
		print('\n===========================')
		print('TEST: autoindex Configs off')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25574"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'sec'
		}
		# try to get
		r = requests.get(URI_TOP + '/content/a/', headers=headers)
		#print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 403)
