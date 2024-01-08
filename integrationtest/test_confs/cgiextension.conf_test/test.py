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

class TestCGIExtensionConfigs(unittest.TestCase):

	def test_cgi_extension_py(self):
		print('\n===========================')
		print('TEST: cgi_extension Configs on, with .py')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25578"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'def'
		}
		# py
		r = requests.get(URI_TOP + '/content/cgi_bin/test.py', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/cgi_bin/test.py') as fs:
			pytext = fs.read()
		self.assertNotEqual(r.content.decode('utf-8'), pytext)
		self.assertEqual(r.status_code, 200)
		# pl
		r = requests.get(URI_TOP + '/content/cgi_bin/cgiext_test.pl', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/cgi_bin/cgiext_test.pl') as fs:
			pltext = fs.read()
		self.assertEqual(r.content.decode('utf-8'), pltext)
		self.assertEqual(r.status_code, 200)
        # sh
		r = requests.get(URI_TOP + '/content/cgi_bin/test.sh', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/cgi_bin/test.sh') as fs:
			shtext = fs.read()
		self.assertEqual(shtext, r.content.decode('utf-8'))
		self.assertEqual(r.status_code, 200)

	def test_cgi_extension_sh(self):
		print('\n===========================')
		print('TEST: cgi_extension Configs on, with sh')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25579"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'secn'
		}
		# py
		r = requests.get(URI_TOP + '/content/cgi_bin/test.py', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/cgi_bin/test.py') as fs:
			pytext = fs.read()
		self.assertEqual(r.content.decode('utf-8'), pytext)
		self.assertEqual(r.status_code, 200)
		# pl
		r = requests.get(URI_TOP + '/content/cgi_bin/cgiext_test.pl', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/cgi_bin/cgiext_test.pl') as fs:
			pltext = fs.read()
		self.assertEqual(r.content.decode('utf-8'), pltext)
		self.assertEqual(r.status_code, 200)
        # sh
		r = requests.get(URI_TOP + '/content/cgi_bin/test.sh', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/cgi_bin/test.sh') as fs:
			shtext = fs.read()
		self.assertNotEqual(shtext, r.content.decode('utf-8'))
		self.assertEqual(r.status_code, 200)

	def test_cgi_extension_pl(self):
		print('\n===========================')
		print('TEST: cgi_extension Configs on, with perl')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25580"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'thr'
		}
		# py
		r = requests.get(URI_TOP + '/content/cgi_bin/test.py', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/cgi_bin/test.py') as fs:
			pytext = fs.read()
		self.assertEqual(r.content.decode('utf-8'), pytext)
		self.assertEqual(r.status_code, 200)
		# pl
		r = requests.get(URI_TOP + '/content/cgi_bin/cgiext_test.pl', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/cgi_bin/cgiext_test.pl') as fs:
			pltext = fs.read()
		self.assertNotEqual(r.content.decode('utf-8'), pltext)
		self.assertEqual(r.status_code, 200)
        # sh
		r = requests.get(URI_TOP + '/content/cgi_bin/test.sh', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		with open('./content/cgi_bin/test.sh') as fs:
			shtext = fs.read()
		self.assertEqual(shtext, r.content.decode('utf-8'))
		self.assertEqual(r.status_code, 200)
