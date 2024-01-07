
import unittest
#from dotenv import load_dotenv
import os
import requests

#load_dotenv()
#HOST_NAME = os.getenv('HOST_NAME')
#PORT = os.getenv('PORT')
HOST_NAME = "localhost"
PORT = "25565"
URI_TOP = 'http://' + HOST_NAME + ':' + PORT

class TestAliasConfigs(unittest.TestCase):

	def test_alias_simple_get(self):
		print('\n===========================')
		print('TEST: Alias Configs with GET')
		print('===========================\n')

		headers = {
			'Host':'def'
		}
		r = requests.get(URI_TOP + '/test/a/', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 200)

	def test_alias_simple_post(self):
		print('\n===========================')
		print('TEST: Alias Configs with POST')
		print('===========================\n')

		with open('./content/wordpress-logo.jpg', 'rb') as fs:
			file_text = fs.read()
		
		headers = {
			'Content-Type':'image/jpeg'
			'Host':'def'
		}
		r = requests.post(URI_TOP + '/test/b/', data=file_text, headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)

		with open('./content/b/0.jpg', 'rb') as fs:
			file_text_cmp = fs.read()
		self.assertEqual(file_text_cmp, file_text)
		self.assertEqual(r.status_code, 201)
		if os.path.exists('./content/b/0.jpg'): os.remove('./content/b/0.jpg')





