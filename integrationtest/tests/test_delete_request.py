import unittest
import os
from dotenv import load_dotenv
from client.Ipv4Client import Ipv4Client
import requests

load_dotenv()
HOST_NAME = os.getenv('HOST_NAME')
PORT = os.getenv('PORT')
URI_TOP = 'http://' + HOST_NAME + ':' + PORT

class TestDeleteRequest(unittest.TestCase):

	def test_simple_post_jpg(self):
		print('\n===========================')
		print('TEST: test simple DELETE request of jpg')
		print('===========================\n')

		with open('./content/wordpress-logo.jpg', 'rb') as fs:
			file_text = fs.read()
		
		headers = {
			'Content-Type':'image/jpeg'
		}
		r = requests.post(URI_TOP, data=file_text, headers=headers)
		r2 = requests.delete(URI_TOP + '/0.jpg')
		print('Response:\n',r2.text)
		print('Status: ',r2.status_code)

		self.assertEqual(os.path.exists('./content/0.jpg'), True)
		self.assertEqual(r2.status_code, 204)

		if os.path.exists('./content/0.jpg'): os.remove('./content/0.jpg')
