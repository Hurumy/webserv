import socket


class BaseClient:
	def __init__(self, timeout:int=10, buffer:int=1024):
		self.__socket = None
		self.__address = None
		self.__timeout = timeout
		self.__buffer = buffer
		self.response = None
		self.status = None

	def connect(self, address, family:int, typ:int, proto:int):
		self.__address = address
		self.__socket = socket.socket(family, typ, proto)
		self.__socket.settimeout(self.__timeout)
		self.__socket.connect(self.__address)

	def send(self, message:str) -> None:
		self.__socket.send(message.encode('utf-8'))

	def recv(self):
		self.response = self.__socket.recv(self.__buffer).decode('utf-8')
		reqline_list = self.response.split(' ')
		if 3 > len(reqline_list): return
		self.status = reqline_list[1]

	def close(self):
		self.__socket.shutdown(socket.SHUT_RDWR)
		self.__socket.close()

class Ipv4Client(BaseClient):
	def __init__(self, host:str, port:int) -> None:
		self.server = (host, port)
		super().__init__(timeout=60, buffer=1024)
		super().connect(self.server, socket.AF_INET, socket.SOCK_STREAM, 0)
