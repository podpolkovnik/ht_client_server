#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#define TCP_SERVER_PORT                     7654
#define MAX_THREADS_NUM                     50

#define TCP_PACKET_SIZE                     256
#define TCP_PACKET_HEADER_TYPE              int
#define TCP_PACKET_STATUS_TYPE              char
#define TCP_PACKET_DATA_SIZE_TYPE           int
#define TCP_PACKET_DATA_TYPE                char
#define TCP_PACKET_DATA_SIZE                    \
        (TCP_PACKET_SIZE                    -   \
        sizeof(TCP_PACKET_HEADER_TYPE)      -   \
        sizeof(TCP_PACKET_STATUS_TYPE)      -   \
        sizeof(TCP_PACKET_DATA_SIZE_TYPE))  /   \
        sizeof(TCP_PACKET_DATA_TYPE)

struct sockaddr_in;

typedef struct ClientHandlerArgs {
    int client_socket_fd;
} ClientHandlerArgs;

#pragma pack(push, 1)
typedef struct TCPPacket {
    TCP_PACKET_HEADER_TYPE      header;
    TCP_PACKET_DATA_SIZE_TYPE   data_size;
    TCP_PACKET_DATA_TYPE        data [TCP_PACKET_DATA_SIZE];
    TCP_PACKET_STATUS_TYPE      status;
} TCPPacket;
#pragma pack(pop)

typedef struct TCPServerConfig {
    unsigned short port;
    unsigned int addr;
    void (*client_connect_callback)(ClientHandlerArgs *args);
    void (*client_disconnect_callback)(ClientHandlerArgs *args);
} TCPServerConfig;

typedef struct TCPServer {
    TCPServerConfig config;
} TCPServer;

void tcpserver_run(TCPServerConfig config);
TCPServerConfig tcpserver_cofnig_create(unsigned short port, unsigned int addr);

#endif // TCP_SERVER_H