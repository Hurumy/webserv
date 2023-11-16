import socket
import select
import time
from random import randint

'''
This 'BaseClient' class is for the integration tests which check the behaviour
of the webserv such as receiving data little by little, HTTP request format's error and
so for. Therefore, this class does not consider to receive larger data than 1024 bytes, and
it lacks some bytes if it happens.
'''
class BaseClient:
	def __init__(self, timeout:int=10, buffer:int=1024):
		self.__socket = None
		self.__address = None
		self.__timeout = timeout
		self.__buffer = buffer
		self.response_data = ''
		self.response_body = ''
		self.status = 0

	def connect(self, address, family:int, typ:int, proto:int):
		self.__address = address
		self.__socket = socket.socket(family, typ, proto)
		self.__socket.settimeout(self.__timeout)
		self.__socket.connect(self.__address)
		self.__socket.setblocking(False)

	def send(self, message:str) -> None:
		self.__socket.send(message.encode('utf-8'))

	def recv(self):
		while (True):
			try:
				self.response_data = self.__socket.recv(self.__buffer).decode('utf-8')
				break
			except:
				continue
		reqline_list = self.response_data.split(' ')
		if 3 > len(reqline_list): return
		self.status = int(reqline_list[1])
		response_lines = self.response_data.split('\r\n')
		is_reach = False
		for i, response_line in enumerate(response_lines):
			if (is_reach == True): self.response_body += response_line
			if (is_reach == True and i != len(response_lines) - 1): self.response_body += '\r\n'
			if (response_line == ''): is_reach = True

	def close(self):
		self.__socket.shutdown(socket.SHUT_RDWR)
		self.__socket.close()

class Ipv4Client(BaseClient):
	def __init__(self, host:str, port:int) -> None:
		self.server = (host, port)
		super().__init__(timeout=60, buffer=1024)
		super().connect(self.server, socket.AF_INET, socket.SOCK_STREAM, 0)
