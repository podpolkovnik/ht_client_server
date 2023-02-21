from tcp.types import *
from tcp.protocol.tcp import TCPPacket


class HCSPPacket(BytesConvertable, StaticSizeType):
    __size =                244
    __cmd_size =            1
    __key_size =            50
    __value_size_size =     4
    __status_size =         1
    __value_size =          \
        __size -            \
        __cmd_size -        \
        __key_size -        \
        __value_size_size - \
        __status_size
    
    def __init__(self):
        self.cmd = BytesBuffer(HCSPPacket.__cmd_size)
        self.key = BytesBuffer(HCSPPacket.__key_size)
        self.value_size = BytesBuffer(HCSPPacket.__value_size_size)
        self.value = BytesBuffer(HCSPPacket.__value_size)
        self.status = BytesBuffer(HCSPPacket.__status_size)

    def to_bytes(self):
        return super().to_bytes([
            *self.cmd.data,
            *self.key.data,
            *self.value_size.data,
            *self.value.data,
            *self.status.data
        ])

    @classmethod
    def from_bytes(cls, data: bytes, offset: int = 0):
        offs = offset
        hcsp = HCSPPacket()
        hcsp.cmd.set([BytesBuffer.from_bytes(data[offs:offs + hcsp.cmd.bytes_size])])
        offs += hcsp.cmd.bytes_size
        hcsp.key.set([BytesBuffer.from_bytes(data[offs:offs + hcsp.key.bytes_size])])
        offs += hcsp.key.bytes_size
        hcsp.value_size.set([BytesBuffer.from_bytes(data[offs:offs + hcsp.value_size.bytes_size])])
        offs += hcsp.value_size.bytes_size
        hcsp.value.set([BytesBuffer.from_bytes(data[offs:offs + hcsp.value.bytes_size])])
        offs += hcsp.value.bytes_size
        hcsp.status.set([BytesBuffer.from_bytes(data[offs:offs + hcsp.status.bytes_size])])
        return hcsp

    @staticmethod
    def sizeof():
        return HCSPPacket.__size


class HCSProtocol:
    class Cmd:
        READ    =   1
        INSERT  =   2
        UPDATE  =   3
        DELETE  =   4
    
    class Status:
        OK =                int(0)
        KEY_NOT_FOUND_ERR = int(1)
        KEY_EXIST_ERR =     int(2)

    @staticmethod
    def exec(cmd: int, key: str, value: BytesConvertable = Int(0)):
        packet = TCPPacket()
        
        hcsp_data = HCSPPacket()
        hcsp_data.cmd.set([Char(cmd)])
        hcsp_data.key.set([String(key)])
        hcsp_data.value_size.set([Int(len(value.to_bytes()))])
        hcsp_data.value.set([value])

        packet.data.set([hcsp_data])

        return packet
        # with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        #     s.connect((HOST, PORT))
        #     s.send(packet.to_bytes())
        #     recv_data = s.recv(TCPPacket.sizeof())
        #     recv_pack = TCPPacket.from_bytes(recv_data)
        #     print('status: ', recv_pack.data.convert_to(HCSPData).status.convert_to(Int))
        #     print('value: ', recv_pack.data.convert_to(HCSPData).value.convert_to(Int))

    @staticmethod
    def read_data(key: str):
        return HCSProtocol.exec(cmd=HCSProtocol.Cmd.READ, key=key)

    @staticmethod
    def insert_data(key: str, value: BytesConvertable):
        return HCSProtocol.exec(cmd=HCSProtocol.Cmd.INSERT, key=key, value=value)

    @staticmethod
    def update_data(key: str, value: BytesConvertable):
        return HCSProtocol.exec(cmd=HCSProtocol.Cmd.UPDATE, key=key, value=value)

    @staticmethod
    def delete_data(key: str):
        return HCSProtocol.exec(cmd=HCSProtocol.Cmd.DELETE, key=key)