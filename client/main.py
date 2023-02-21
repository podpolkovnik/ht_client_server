from api import RemoteHashTable


HOST = "127.0.0.1"
PORT = 8096


def run():
    ht = RemoteHashTable(HOST, PORT)
    ht.delete('test')
    ht.insert('test', 'Haahahaha')
    ht.delete('test')
    ht.insert('test', 'fafa')
    bb = ht['test']
    print(bb)
    for b in bb:
        print(b)


if __name__ == '__main__':
    run()