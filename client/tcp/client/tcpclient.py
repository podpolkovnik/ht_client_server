import socket

from ..protocol.tcp import TCPPacket


class TCPClient:
    def __init__(self, server_addr: str, server_port: int):
        self.s_addr = server_addr
        self.s_port = server_port

    def connect(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((self.s_addr, self.s_port))

    def send(self, tcp_packet: TCPPacket):
        return self.socket.send(tcp_packet.to_bytes())

    def recv(self):
        data = self.socket.recv(TCPPacket.sizeof())
        return TCPPacket.from_bytes(data=data)
    
    def disconnect(self):
        self.socket.close()

