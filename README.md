# Remote Hash Table

## Install Server

```
git clone https://github.com/podpolkovnik/ht_client_server.git
cd server
make
```
## Usage
### To start the server
```
cd server
./htserver
```
### To start the client
```
cd client
python3 main.py 
```
### Examples
Insert, update, delete and read a key-value pair into/from Remote Hash Table
#### 1. Using python Remote Hash Table API create the python project: 
``` Python
# main.py
from api import RemoteHashTable


HOST = "127.0.0.1"
PORT = 8096


def run():
    ht = RemoteHashTable(HOST, PORT)
    key = 'test'
    value = 1000
    new_value = 'new value'
    # For insert key-value pair
    ht.insert(key, value)
    # For update key-value pair by key
    ht.update(key, new_value)
    # For read value by key (you can also use "resulst = ht[key]")
    result = ht.read(key)
    print(result)
    # For delete key-value pair by key
    ht.delete(key)
    

if __name__ == '__main__':
    run()
```
#### 2. Start the server
```
./htserver
```
#### 3. Execute python file
```
python3 main.py
```
