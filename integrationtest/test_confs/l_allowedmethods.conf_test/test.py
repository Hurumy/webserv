
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

class TestL_AllowedMethodsConfigs(unittest.TestCase):

	def test_l_allowed_methods_only_get(self):
		print('\n===========================')
		print('TEST: l_allowedMethods Configs with only-GET')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25570"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'def'
		}
		# try to get
		r = requests.get(URI_TOP + '/content/a/index.html', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, "a\n")
		self.assertEqual(r.status_code, 200)

		# try to post
		with open('./content/wordpress-logo.jpg', 'rb') as fs:
			file_text = fs.read()
		headers2 = {
			'Content-Type':'image/jpeg',
			'Host':'def'
		}
		r2 = requests.post(URI_TOP + '/content/a/', data=file_text, headers=headers2)
		print('Response:\n',r2.text)
		print('Status: ',r2.status_code)
		self.assertEqual(r2.status_code, 405)
		if os.path.exists('./content/a/0.jpg'): os.remove('./content/a/0.jpg')

		# try to delete
		if not os.path.exists('./content/a/dummy'):
			f = open("./content/a/dummy", 'w')
			f.close()
		r3 = requests.delete(URI_TOP + '/content/a/dummy', headers=headers)
		print('Response:\n',r3.text)
		print('Status: ',r3.status_code)
		self.assertEqual(os.path.exists('./content/a/dummy'), True)
		self.assertEqual(r3.status_code, 405)

	def test_l_allowed_methods_only_post(self):
		print('\n===========================')
		print('TEST: l_allowedMethods Configs with only-POST')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25570"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'def'
		}
		# try to get
		r = requests.get(URI_TOP + '/content/b/index.html', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 405)

		# try to post
		with open('./content/wordpress-logo.jpg', 'rb') as fs:
			file_text = fs.read()
		headers2 = {
			'Content-Type':'image/jpeg',
			'Host':'def'
		}
		r2 = requests.post(URI_TOP + '/content/b/', data=file_text, headers=headers2)
		print('Response:\n',r2.text)
		print('Status: ',r2.status_code)
		with open('./content/b/0.jpg', 'rb') as fs:
			file_text_cmp = fs.read()
		self.assertEqual(file_text_cmp, file_text)
		self.assertEqual(r2.status_code, 201)
		if os.path.exists('./content/b/0.jpg'): os.remove('./content/b/0.jpg')

		# try to delete
		if not os.path.exists('./content/b/dummy'):
			f = open("./content/b/dummy", 'w')
			f.close()
		r3 = requests.delete(URI_TOP + '/content/b/dummy', headers=headers)
		print('Response:\n',r3.text)
		print('Status: ',r3.status_code)
		self.assertEqual(os.path.exists('./content/b/dummy'), True)
		self.assertEqual(r3.status_code, 405)

	def test_l_allowed_methods_only_delete(self):
		print('\n===========================')
		print('TEST: l_allowedMethods Configs with only-DELETE')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25570"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'def'
		}
		# try to get
		r = requests.get(URI_TOP + '/content/c/index.html', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 405)

		# try to post
		with open('./content/wordpress-logo.jpg', 'rb') as fs:
			file_text = fs.read()
		headers2 = {
			'Content-Type':'image/jpeg',
			'Host':'def'
		}
		r2 = requests.post(URI_TOP + '/content/c/', data=file_text, headers=headers2)
		print('Response:\n',r2.text)
		print('Status: ',r2.status_code)
		self.assertEqual(r2.status_code, 405)
		if os.path.exists('./content/c/0.jpg'): os.remove('./content/c/0.jpg')

		# try to delete
		if not os.path.exists('./content/c/dummy'):
			f = open("./content/c/dummy", 'w')
			f.close()	
		r3 = requests.delete(URI_TOP + '/content/c/dummy', headers=headers)
		print('Response:\n',r3.text)
		print('Status: ',r3.status_code)
		self.assertEqual(os.path.exists('./content/c/dummy'), False)
		self.assertEqual(r3.status_code, 204)

	def test_l_allowed_methods_only_get_multi_server(self):
		print('\n===========================')
		print('TEST: l_allowedMethods Configs with only-GET, multi server')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25571"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'sec'
		}
		# try to get
		r = requests.get(URI_TOP + '/content/b/index.html', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.text, "b\n")
		self.assertEqual(r.status_code, 200)

		# try to post
		with open('./content/wordpress-logo.jpg', 'rb') as fs:
			file_text = fs.read()
		headers2 = {
			'Content-Type':'image/jpeg',
			'Host':'sec'
		}
		r2 = requests.post(URI_TOP + '/content/b/', data=file_text, headers=headers2)
		print('Response:\n',r2.text)
		print('Status: ',r2.status_code)
		self.assertEqual(r2.status_code, 405)
		if os.path.exists('./content/b/0.jpg'): os.remove('./content/b/0.jpg')

		# try to delete
		if not os.path.exists('./content/b/dummy'):
			f = open("./content/b/dummy", 'w')
			f.close()
		r3 = requests.delete(URI_TOP + '/content/b/dummy', headers=headers)
		print('Response:\n',r3.text)
		print('Status: ',r3.status_code)
		self.assertEqual(os.path.exists('./content/b/dummy'), True)
		self.assertEqual(r3.status_code, 405)

	def test_l_allowed_methods_only_post_multi_server(self):
		print('\n===========================')
		print('TEST: l_allowedMethods Configs with only-POST, multi server')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25571"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'sec'
		}
		# try to get
		r = requests.get(URI_TOP + '/content/c/index.html', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 405)

		# try to post
		with open('./content/wordpress-logo.jpg', 'rb') as fs:
			file_text = fs.read()
		headers2 = {
			'Content-Type':'image/jpeg',
			'Host':'sec'
		}
		r2 = requests.post(URI_TOP + '/content/c/', data=file_text, headers=headers2)
		print('Response:\n',r2.text)
		print('Status: ',r2.status_code)
		with open('./content/c/0.jpg', 'rb') as fs:
			file_text_cmp = fs.read()
		self.assertEqual(file_text_cmp, file_text)
		self.assertEqual(r2.status_code, 201)
		if os.path.exists('./content/c/0.jpg'): os.remove('./content/c/0.jpg')

		# try to delete
		if not os.path.exists('./content/c/dummy'):
			f = open("./content/c/dummy", 'w')
			f.close()
		r3 = requests.delete(URI_TOP + '/content/c/dummy', headers=headers)
		print('Response:\n',r3.text)
		print('Status: ',r3.status_code)
		self.assertEqual(os.path.exists('./content/c/dummy'), True)
		self.assertEqual(r3.status_code, 405)

	def test_l_allowed_methods_only_delete_multi_server(self):
		print('\n===========================')
		print('TEST: l_allowedMethods Configs with only-DELETE, multi server')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25571"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'sec'
		}
		# try to get
		r = requests.get(URI_TOP + '/content/a/index.html', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 405)

		# try to post
		with open('./content/wordpress-logo.jpg', 'rb') as fs:
			file_text = fs.read()
		headers2 = {
			'Content-Type':'image/jpeg',
			'Host':'sec'
		}
		r2 = requests.post(URI_TOP + '/content/a/', data=file_text, headers=headers2)
		print('Response:\n',r2.text)
		print('Status: ',r2.status_code)
		self.assertEqual(r2.status_code, 405)
		if os.path.exists('./content/a/0.jpg'): os.remove('./content/a/0.jpg')

		# try to delete
		if not os.path.exists('./content/a/dummy'):
			f = open("./content/a/dummy", 'w')
			f.close()	
		r3 = requests.delete(URI_TOP + '/content/a/dummy', headers=headers)
		print('Response:\n',r3.text)
		print('Status: ',r3.status_code)
		self.assertEqual(os.path.exists('./content/a/dummy'), False)
		self.assertEqual(r3.status_code, 204)

	def test_l_allowed_methods_priority(self):
		print('\n===========================')
		print('TEST: l_allowedMethods Configs priority')
		print('===========================\n')

		HOST_NAME = "localhost"
		PORT = "25572"
		URI_TOP = 'http://' + HOST_NAME + ':' + PORT
		headers = {
			'Host':'thr'
		}
		# try to get
		r = requests.get(URI_TOP + '/content/a/index.html', headers=headers)
		print('Response:\n',r.text)
		print('Status: ',r.status_code)
		self.assertEqual(r.status_code, 405)

		# try to post
		with open('./content/wordpress-logo.jpg', 'rb') as fs:
			file_text = fs.read()
		headers2 = {
			'Content-Type':'image/jpeg',
			'Host':'thr'
		}
		r2 = requests.post(URI_TOP + '/content/a/', data=file_text, headers=headers2)
		print('Response:\n',r2.text)
		print('Status: ',r2.status_code)
		with open('./content/a/0.jpg', 'rb') as fs:
			file_text_cmp = fs.read()
		self.assertEqual(file_text_cmp, file_text)
		self.assertEqual(r2.status_code, 201)
		if os.path.exists('./content/a/0.jpg'): os.remove('./content/a/0.jpg')

		# try to delete
		if not os.path.exists('./content/a/dummy'):
			f = open("./content/a/dummy", 'w')
			f.close()
		r3 = requests.delete(URI_TOP + '/content/a/dummy', headers=headers)
		print('Response:\n',r3.text)
		print('Status: ',r3.status_code)
		self.assertEqual(os.path.exists('./content/a/dummy'), True)
		self.assertEqual(r3.status_code, 405)

