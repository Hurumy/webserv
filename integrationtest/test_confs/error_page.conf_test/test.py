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

class TestErrorPageConfigs(unittest.TestCase):

	def test_errorpage_404(self):
		print('\n===========================')
		print('TEST: Error page Configs on 404')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25583"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'def'
		}
		# 404
		with open('./content/a/index.html') as fs:
			texta = fs.read()
		r = requests.get(URI_TOP + '/content/a/awkdnkajdnjkan', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.content.decode('utf-8'), texta)
		self.assertEqual(r.status_code, 404)

	def test_errorpage_403(self):
		print('\n===========================')
		print('TEST: Error page Configs 403')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25583"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'def'
		}
		# 403
		with open('./content/b/index.html') as fs:
			textb = fs.read()
		r2 = requests.get(URI_TOP + '/content/b/forbidden', headers=headers)
		print('Response:\n',r2.text)
		print('Status: ',r2.status_code)
		self.assertEqual(r2.content.decode('utf-8'), textb)
		self.assertEqual(r2.status_code, 403)

	def test_errorpage_405(self):
		print('\n===========================')
		print('TEST: Error page Configs 405')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25583"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'def'
		}
		# 405
		with open('./content/c/index.html') as fs:
			textc = fs.read()
		r2 = requests.head(URI_TOP + '/content/c/dummy', headers=headers)
		print('Response:\n',r2.text)
		print('Status: ',r2.status_code)
		self.assertEqual(r2.content.decode('utf-8'), textc)
		self.assertEqual(r2.status_code, 405)

