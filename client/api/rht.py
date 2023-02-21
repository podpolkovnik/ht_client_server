from typing import Any

from tcp.types import *
from tcp.client import TCPClient
from tcp.protocol.hcs import HCSProtocol, HCSPPacket
from tcp.protocol.tcp import TCPPacket

class RemoteHashTable:
    def __init__(self, server_addr: str, server_port: int):
        self.tcp_client = TCPClient(server_addr, server_port)

    def __getitem__(self, key: str):
        self.tcp_client.connect()
        res_packet = HCSProtocol.read_data(key=key)
        self.tcp_client.send(res_packet)
        recv_packet = self.tcp_client.recv()
        self.__handle_status(key, recv_packet)
        val_size = recv_packet.data.convert_to(HCSPPacket).value_size.convert_to(Int)
        val_bytes = recv_packet.data.convert_to(HCSPPacket).value.to_bytes()
        res = bytes(bytearray(val_bytes[0:val_size.data]))
        self.tcp_client.disconnect()
        return res
    
    def read(self, key: str):
        return self[key]

    def insert(self, key: str, value: Any):
        self.tcp_client.connect()
        stype = SupportedTypes(type=type(value))
        if stype != None:
            val = stype(value)
            res_packet = HCSProtocol.insert_data(key=key, value=val)
            self.tcp_client.send(res_packet)
            recv_packet = self.tcp_client.recv()
            self.__handle_status(key, recv_packet)
        self.tcp_client.disconnect()
    
    def update(self, key: str, value: Any):
        self.tcp_client.connect()
        stype = SupportedTypes(type=type(value))
        if stype != None:
            val = stype(value)
            res_packet = HCSProtocol.update_data(key=key, value=val)
            self.tcp_client.send(res_packet)
            recv_packet = self.tcp_client.recv()
            self.__handle_status(key, recv_packet)
        self.tcp_client.disconnect()

    def delete(self, key: str):
        self.tcp_client.connect()
        res_packet = HCSProtocol.delete_data(key=key)
        self.tcp_client.send(res_packet)
        recv_packet = self.tcp_client.recv()
        self.__handle_status(key, recv_packet)
        self.tcp_client.disconnect()

    def __handle_status(self, key: str, tcp_packet: TCPPacket):
        status = tcp_packet.data.convert_to(HCSPPacket).status.convert_to(Int)

        if status.data == HCSProtocol.Status.OK:
            return HCSProtocol.Status.OK
        elif status.data == HCSProtocol.Status.KEY_NOT_FOUND_ERR:
            print(f'Error! Key "{key}" not found in table\n')
            return HCSProtocol.Status.KEY_NOT_FOUND_ERR
        elif status.data == HCSProtocol.Status.KEY_EXIST_ERR:
            print(f'Error! Key "{key}" already exist in table\n')
            return HCSProtocol.Status.KEY_EXIST_ERR
