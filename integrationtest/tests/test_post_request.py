import unittest
import os
from dotenv import load_dotenv
from client.Ipv4Client import Ipv4Client
import requests

load_dotenv()
HOST_NAME = os.getenv('HOST_NAME')
PORT = os.getenv('PORT')
URI_TOP = 'http://' + HOST_NAME + ':' + PORT

class TestPostRequest(unittest.TestCase):

	def test_simple_post_jpg(self):
		print('\n===========================')
		print('TEST: test simple POST request of jpg')
		print('===========================\n')

		with open('./content/wordpress-logo.jpg', 'rb') as fs:
			file_text = fs.read()
		
		headers = {
			'Content-Type':'image/jpeg'
		}
		r = requests.post(URI_TOP, data=file_text)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)

		with open('./content/0.jpg', 'rb') as fs:
			file_text_cmp = fs.read()
		self.assertEqual(file_text_cmp, file_text)
		self.assertEqual(r.status_code, 201)

		if os.path.exists('./content/0.jpg'): os.remove('./content/0.jpg')

	def test_trailerfield(self):
		print('\n===========================')
		print('TEST: test trailerfield Chunked transfer encoding')
		print('===========================\n')

		with open('./data/chunked_trailer', 'rb') as fs:
			file_text = fs.read()
		headers = {
			'Content-Type':'text/plain',
			'Transfer-Encoding':'chunked',
			'Trailer':'Expires'
		}
		r = requests.post(URI_TOP, data=file_text, headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)

		with open('./content/0.txt', 'rb') as fs:
			file_text_cmp = fs.read()
		self.assertEqual(file_text_cmp.decode('ascii'), 'MozillaDeveloperNetwork')
		self.assertEqual(r.status_code, 201)

		if os.path.exists('./content/0.txt'): os.remove('./content/0.txt')