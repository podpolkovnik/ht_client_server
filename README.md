# Remote Hash Table

## Install Server

```
git clone https://github.com/podpolkovnik/ht_client_server.git
cd ht_client_server/server/
make
```
## Usage
### To start the server
```
cd ht_client_server/server/
./htserver
```
### To start the client
```
cd ht_client_server/client/
python3 main.py 
```
### Examples
Insert, update, delete and read a key-value pair into/from Remote Hash Table
#### 1. Using python Remote Hash Table API create the python project 
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
## Description
> **Note**
> At the moment, the server can only process ```read```, ```insert```, ```update``` and ```delete``` commands

### Client-Server diagram
![alt text](https://github.com/podpolkovnik/ht_client_server/blob/main/blob/client_server_diagram.png?raw=true)
### TCP Packet structure
> **Note**
> In this scheme, the TCP Packet size is 256 bytes, but you can change the size in the code (flie ```server/include/tcpserver.h```)
> ``` C
> // ...
> #define TCP_PACKET_SIZE                     256
> #define TCP_PACKET_HEADER_TYPE              int
> #define TCP_PACKET_STATUS_TYPE              char
> #define TCP_PACKET_DATA_SIZE_TYPE           int
> #define TCP_PACKET_DATA_TYPE                char
> // ...
> ```

![alt text](https://github.com/podpolkovnik/ht_client_server/blob/main/blob/tcp_hcsp_packets_diagram.png?raw=true)

## TODO
1. Implement multi-packet data transfer if they do not fit into one TCP Packet
2. Choose the optimal hash function and other parameters for the Hash Table to increase performance
3. Optimize the code
4. Write documentation
