from ..types import *


class TCPPacket(BytesConvertable, StaticSizeType):
    __size =                256
    __header_size =         4
    __data_len_size =       4
    __status_size =         4
    __data_size =           \
        __size -            \
        __header_size -     \
        __data_len_size -   \
        __status_size

    def __init__(self):
        self.__header = BytesBuffer(TCPPacket.__header_size)
        self.__data_len = BytesBuffer(TCPPacket.__data_len_size)
        self.__data = BytesBuffer(TCPPacket.__data_size)
        self.__status = BytesBuffer(TCPPacket.__status_size)

    @property
    def header(self):
        return self.__header

    @property
    def data_len(self):
        return self.__data_len

    @property
    def data(self):
        return self.__data

    @property
    def status(self):
        return self.__status

    def to_bytes(self):
        res = super().to_bytes([
            *self.header.data,
            *self.data_len.data,
            *self.data.data,
            *self.status.data,
            ])
        return res

    @classmethod
    def from_bytes(cls, data: bytes, offset: int = 0):
        offs = offset
        packet = TCPPacket()
        packet.header.set([BytesBuffer.from_bytes(data[offs:offs + packet.header.bytes_size])])
        offs += packet.header.bytes_size
        packet.data_len.set([BytesBuffer.from_bytes(data[offs:offs + packet.data_len.bytes_size])])
        offs += packet.data_len.bytes_size
        packet.data.set([BytesBuffer.from_bytes(data[offs:offs + packet.data.bytes_size])])
        offs += packet.data.bytes_size
        packet.status.set([BytesBuffer.from_bytes(data[offs:offs + packet.status.bytes_size])])
        return packet

    @staticmethod
    def sizeof():
        return TCPPacket.__size
    

class TCPProtocol:
    pass