#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "tcpserver.h"
#include "hcsprotocol.h"
#include "hashtable.h"
#include "value.h"

void tcpserver_run(TCPServerConfig config) {
    int socket_fd;
    struct sockaddr_in server_address;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
	server_address.sin_port = config.port;
	server_address.sin_addr.s_addr = config.addr;

    const int enable = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("Set socket option SO_REUSEADDR failed\n");

    if (bind(socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        perror("Socket bind error\n");
        return;
    }

    if (listen(socket_fd, MAX_THREADS_NUM) == -1) {
        perror("Socket listen error\n");
        close(socket_fd);
        return;
    }

    pthread_t tid[MAX_THREADS_NUM];
    int i = 0;
    ClientHandlerArgs args;

    while (1) {
        int client_socket_fd = accept(socket_fd, NULL, NULL);
        args.client_socket_fd = client_socket_fd;

        if (client_socket_fd == -1) {
            perror("Accept socket error\n");
            break;
        }
        
        void (*client_connect_callback)(ClientHandlerArgs *args) =
            config.client_connect_callback;
        if (client_connect_callback) {
            if(pthread_create(&tid[i++], NULL, client_connect_callback, &args) != 0) {
                perror("Thread create error\n");
                break;
            }
        }

        if(i >= MAX_THREADS_NUM) {
            i = 0;
            while(i < MAX_THREADS_NUM) {
                pthread_join(tid[i++], NULL);
            }
        }
    }
    close(socket_fd);
}

TCPServerConfig tcpserver_cofnig_create(unsigned short port, unsigned int addr) {
    TCPServerConfig config;
    if (addr == 0) {
        config.addr = INADDR_ANY;
    } else {
        config.addr = addr;
    }
    config.port = port;
    config.client_connect_callback = NULL;
    config.client_disconnect_callback = NULL;
    return config;
}