import struct
from typing import Any, List


class BytesConvertable:
    def to_bytes(self, data: list):
        res = bytearray()
        for btval in data:
            if isinstance(btval, BaseType):
                res.extend(struct.pack(btval.pack_fmt(), btval.data))
        return bytes(res)

    def convert_to(self, type):
        res = bytearray()
        ldata = self.to_bytes()
        rlen = type.sizeof() if issubclass(type, StaticSizeType) else len(ldata)
        if issubclass(type, BytesConvertable):
            for i in range(rlen):
                if issubclass(type, StaticSizeType):
                    if i + 1 > len(ldata):
                        res.extend(bytes(type.sizeof() - len(ldata)))
                        break
                res.extend(int.to_bytes(ldata[i], length=1, byteorder='little'))
        return type.from_bytes(bytes(res))

    @classmethod
    def from_bytes(cls, data: bytes, offset: int = 0):
        if issubclass(cls, BaseType):
            return cls(struct.unpack(cls.pack_fmt(), bytearray(data[offset:offset+cls.sizeof()]))[0])
        raise Exception(f'Func: <from_bytes>, error class type: {cls}')

    @classmethod
    def pack_fmt():
        raise NotImplementedError('BytesConvertable.pack_fmt()')


class StaticSizeType:
    @staticmethod 
    def sizeof():
        raise NotImplementedError('StaticSizeType.sizeof()')


class BaseType(BytesConvertable, StaticSizeType):
    def __init__(self, data: Any, signed: bool = True):
        self.data = data
        self.signed = signed

    @staticmethod
    def sizeof():
        raise NotImplementedError('BaseType.sizeof()')

    @staticmethod
    def pack_fmt():
        raise NotImplementedError('BaseType.pack_fmt()')

    def to_bytes(self):
        return super().to_bytes([self])

    def __str__(self):
        return str(self.data)

    def __repr__(self):
        return str(self.data)


class BytesBuffer(BytesConvertable):
    def __init__(self, size: int):
        self.data = [UChar(0) for i in range(size)]
        self.bytes_size = size
    
    def set(self, data: List[BytesConvertable]):
        data_size = 0
        for d in data:
            data_size += len(d.to_bytes())
        if data_size > self.bytes_size:
            return
        self.data.clear()
        for d in data:
            bytes_val = d.to_bytes()
            for byte in bytes_val:
                self.data.append(UChar(byte))
        self.data.extend([UChar(0) for i in range(self.bytes_size - data_size)])

    def iter_unpack(self, base_types_list: List[BaseType]):
        res = []
        offset = 0
        bytes = self.to_bytes()

        for type in base_types_list:
            res.append(type.from_bytes(bytes, offset))
            offset += type.sizeof()
        return res

    def to_bytes(self):
        return super().to_bytes(self.data)

    @classmethod
    def from_bytes(cls, data: bytes, offset: int = 0):
        res = BytesBuffer(len(data))
        bb_data = []
        for byte in data:
            bb_data.append(UChar(byte))
        res.set(bb_data)
        return res


class Int(BaseType):
    __byte_size = 4
    __pack_fmt = 'i'

    def __init__(self, data: int = 0):
        super().__init__(data=data)

    @staticmethod
    def sizeof():
        return Int.__byte_size

    @staticmethod
    def pack_fmt():
        return Int.__pack_fmt


class UInt(BaseType):
    __byte_size = 4
    __pack_fmt = 'I'

    def __init__(self, data: int = 0):
        super().__init__(data=data, signed=False)

    @staticmethod
    def sizeof():
        return UInt.__byte_size

    @staticmethod
    def pack_fmt():
        return UInt.__pack_fmt


class Float(BaseType):
    __byte_size = 4
    __pack_fmt = 'f'

    def __init__(self, data: float = 0.0):
        super().__init__(data=data)

    @staticmethod
    def sizeof():
        return Float.__byte_size

    @staticmethod
    def pack_fmt():
        return Float.__pack_fmt


class Char(BaseType):
    __byte_size = 1
    __pack_fmt = 'b'

    def __init__(self, data: int = 0):
        super().__init__(data=data)

    @staticmethod
    def sizeof():
        return Char.__byte_size

    @staticmethod
    def pack_fmt():
        return Char.__pack_fmt


class UChar(BaseType):
    __byte_size = 1
    __pack_fmt = 'B'

    def __init__(self, data: int = 0):
        super().__init__(data=data)

    @staticmethod
    def sizeof():
        return UChar.__byte_size

    @staticmethod
    def pack_fmt():
        return UChar.__pack_fmt


class String(BytesConvertable):
    def __init__(self, data: str = ""):
        self.data = [Char(ord(i)) for i in data]

    def to_bytes(self):
        return super().to_bytes(self.data)

    def append(self, sym: str | bytes | bytearray):
        self.data.append(Char(ord(sym)))

    @classmethod
    def from_bytes(cls, data: bytes, offset: int = 0):
        res = String()
        for byte in data:
            res.append(chr(byte))
        return res

    def __repr__(self):
        res = ''
        for byte in self.data:
            res += chr(byte.data)
        return res
    

class SupportedTypes:
    __types = {
        int: Int,
        float: Float,
        str: String
    }

    def __new__(cls, type):
        if type in SupportedTypes.__types.keys():
            return SupportedTypes.__types[type]
        return None