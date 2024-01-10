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

class TestIndexConfigs(unittest.TestCase):

	def test_index_first(self):
		print('\n===========================')
		print('TEST: Index Configs priority test')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25586"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'def'
		}
		with open('./content/a/only_a.html') as fs:
			texta = fs.read()
		r = requests.get(URI_TOP + '/content/a', headers=headers)
		#print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.content.decode('utf-8'), texta)
		self.assertEqual(r.status_code, 200)

	def test_index_second(self):
		print('\n===========================')
		print('TEST: Index Configs priority test: secondary')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25586"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'def'
		}
		with open('./content/b/only_b.html') as fs:
			texta = fs.read()
		r = requests.get(URI_TOP + '/content/b', headers=headers)
		#print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.content.decode('utf-8'), texta)
		self.assertEqual(r.status_code, 200)

	def test_index_third(self):
		print('\n===========================')
		print('TEST: Index Configs priority test: third')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25586"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'def'
		}
		with open('./content/c/index.html') as fs:
			texta = fs.read()
		r = requests.get(URI_TOP + '/content/c', headers=headers)
		#print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.content.decode('utf-8'), texta)
		self.assertEqual(r.status_code, 200)
