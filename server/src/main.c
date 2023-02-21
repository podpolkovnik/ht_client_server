#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "hashtable.h"
#include "hcsprotocol.h"
#include "value.h"
#include "array.h"
#include "tcpserver.h"

#include "new.h"

static HashTable *HT;
pthread_mutex_t mutex;

void client_handler(ClientHandlerArgs *args) {
    TCPPacket recv_packet;
    recv(args->client_socket_fd, &recv_packet, TCP_PACKET_SIZE, 0);
    
    pthread_mutex_lock(&mutex);
    TCPPacket hpacket = hcsp_handle_packet(HT, &recv_packet);
    send(args->client_socket_fd, &hpacket, sizeof(TCPPacket), 0);
    pthread_mutex_unlock(&mutex);

    close(args->client_socket_fd);
    pthread_exit(NULL);
}

int main() {
    HT = hashtable_create();
    pthread_mutex_init(&mutex, NULL);

    TCPServerConfig config = tcpserver_cofnig_create(htons(8096), 0);
    config.client_connect_callback = client_handler;
    tcpserver_run(config);

    pthread_mutex_destroy(&mutex);

    return 0;
}