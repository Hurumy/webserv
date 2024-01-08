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

class TestLCGIExtensionConfigs(unittest.TestCase):

	def test_cgi_extension_fst(self):
		print('\n===========================')
		print('TEST: l_cgi_extension Configs on, with each Extensions')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25581"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'def'
		}
		# a
		# py
		r = requests.get(URI_TOP + '/content/a/test.py', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/a/test.py') as fs:
			pytext = fs.read()
		self.assertNotEqual(r.content.decode('utf-8'), pytext)
		self.assertEqual(r.status_code, 200)
		# pl
		r = requests.get(URI_TOP + '/content/a/cgiext_test.pl', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/a/cgiext_test.pl') as fs:
			pltext = fs.read()
		self.assertEqual(r.content.decode('utf-8'), pltext)
		self.assertEqual(r.status_code, 200)
		# sh
		r = requests.get(URI_TOP + '/content/a/test.sh', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/a/test.sh') as fs:
			shtext = fs.read()
		self.assertEqual(shtext, r.content.decode('utf-8'))
		self.assertEqual(r.status_code, 200)

		# b
		# py
		r = requests.get(URI_TOP + '/content/b/test.py', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/b/test.py') as fs:
			pytext = fs.read()
		self.assertEqual(r.content.decode('utf-8'), pytext)
		self.assertEqual(r.status_code, 200)
		# pl
		r = requests.get(URI_TOP + '/content/b/cgiext_test.pl', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/b/cgiext_test.pl') as fs:
			pltext = fs.read()
		self.assertEqual(r.content.decode('utf-8'), pltext)
		self.assertEqual(r.status_code, 200)
		# sh
		r = requests.get(URI_TOP + '/content/b/test.sh', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/b/test.sh') as fs:
			shtext = fs.read()
		self.assertNotEqual(shtext, r.content.decode('utf-8'))
		self.assertEqual(r.status_code, 200)

		# c
		# py
		r = requests.get(URI_TOP + '/content/c/test.py', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/c/test.py') as fs:
			pytext = fs.read()
		self.assertEqual(r.content.decode('utf-8'), pytext)
		self.assertEqual(r.status_code, 200)
		# pl
		r = requests.get(URI_TOP + '/content/c/cgiext_test.pl', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/c/cgiext_test.pl') as fs:
			pltext = fs.read()
		self.assertNotEqual(r.content.decode('utf-8'), pltext)
		self.assertEqual(r.status_code, 200)
		# sh
		r = requests.get(URI_TOP + '/content/c/test.sh', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/c/test.sh') as fs:
			shtext = fs.read()
		self.assertEqual(shtext, r.content.decode('utf-8'))
		self.assertEqual(r.status_code, 200)

	def test_cgi_extension_secn(self):
		print('\n===========================')
		print('TEST: l_cgi_extension Configs on, with each Extensions, another pattern')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25582"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'secn'
		}
		# b
		# py
		r = requests.get(URI_TOP + '/content/b/test.py', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/b/test.py') as fs:
			pytext = fs.read()
		self.assertNotEqual(r.content.decode('utf-8'), pytext)
		self.assertEqual(r.status_code, 200)
		# pl
		r = requests.get(URI_TOP + '/content/b/cgiext_test.pl', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/b/cgiext_test.pl') as fs:
			pltext = fs.read()
		self.assertEqual(r.content.decode('utf-8'), pltext)
		self.assertEqual(r.status_code, 200)
		# sh
		r = requests.get(URI_TOP + '/content/b/test.sh', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/b/test.sh') as fs:
			shtext = fs.read()
		self.assertEqual(shtext, r.content.decode('utf-8'))
		self.assertEqual(r.status_code, 200)

		# c
		# py
		r = requests.get(URI_TOP + '/content/c/test.py', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/c/test.py') as fs:
			pytext = fs.read()
		self.assertEqual(r.content.decode('utf-8'), pytext)
		self.assertEqual(r.status_code, 200)
		# pl
		r = requests.get(URI_TOP + '/content/c/cgiext_test.pl', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/c/cgiext_test.pl') as fs:
			pltext = fs.read()
		self.assertEqual(r.content.decode('utf-8'), pltext)
		self.assertEqual(r.status_code, 200)
		# sh
		r = requests.get(URI_TOP + '/content/c/test.sh', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/c/test.sh') as fs:
			shtext = fs.read()
		self.assertNotEqual(shtext, r.content.decode('utf-8'))
		self.assertEqual(r.status_code, 200)

		# a
		# py
		r = requests.get(URI_TOP + '/content/a/test.py', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/a/test.py') as fs:
			pytext = fs.read()
		self.assertEqual(r.content.decode('utf-8'), pytext)
		self.assertEqual(r.status_code, 200)
		# pl
		r = requests.get(URI_TOP + '/content/a/cgiext_test.pl', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/a/cgiext_test.pl') as fs:
			pltext = fs.read()
		self.assertNotEqual(r.content.decode('utf-8'), pltext)
		self.assertEqual(r.status_code, 200)
		# sh
		r = requests.get(URI_TOP + '/content/a/test.sh', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/a/test.sh') as fs:
			shtext = fs.read()
		self.assertEqual(shtext, r.content.decode('utf-8'))
		self.assertEqual(r.status_code, 200)

