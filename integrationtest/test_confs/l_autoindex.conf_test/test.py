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

class TestLAutoIndexConfigs(unittest.TestCase):

	def test_l_autoindex_prim(self):
		print('\n===========================')
		print('TEST: l_autoindex Configs on')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25575"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'def'
		}
		# try to get
		r = requests.get(URI_TOP + '/content/a/', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 200)
		# try to get
		r = requests.get(URI_TOP + '/content/b/', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 403)

	def test_l_autoindex_secn(self):
		print('\n===========================')
		print('TEST: l_autoindex Configs off')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25576"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'sec'
		}
		# try to get
		r = requests.get(URI_TOP + '/content/a/', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 403)
		# try to get
		r = requests.get(URI_TOP + '/content/b/', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 200)

	def test_l_autoindex_priority(self):
		print('\n===========================')
		print('TEST: l_autoindex Configs Priority')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25577"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'thr'
		}
		# try to get
		r = requests.get(URI_TOP + '/content/a/', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 200)
