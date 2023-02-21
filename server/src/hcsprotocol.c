#include <string.h>

#include "hashtable.h"
#include "hcsprotocol.h"
#include "tcpserver.h"
#include "value.h"

#include "new.h"

void hcsp_unpack(HCSPData *dest, TCPPacket *packet) {
    char data[TCP_PACKET_DATA_SIZE];

    memcpy(&data, &(packet->data), TCP_PACKET_DATA_SIZE);
    memcpy(dest, &data, HCSP_DATA_SIZE);
}

void hcsp_pack(TCPPacket *dest, Value *value, char status) {
    HCSPData hcsp_data;
    memset(&hcsp_data, 0, HCSP_DATA_SIZE);
    if (value) {
        unsigned long value_size = value->size;
        void *data = value->data;
        memcpy(&hcsp_data.value_size, &value_size, sizeof(HCSP_DATA_VALUE_SIZE_TYPE));
        memcpy(&hcsp_data.value, data, value_size);
    }
    memcpy(&hcsp_data.status, &status, sizeof(status));
    memcpy(dest->data, &hcsp_data, HCSP_DATA_SIZE);
}

TCPPacket hcsp_handle_packet(HashTable *ht, TCPPacket *packet) {
    HCSPData hcsp_data;
    hcsp_unpack(&hcsp_data, packet);

    int cmd = hcsp_data.cmd;
    const char *key = hcsp_data.key;
    void *value = &hcsp_data.value;
    unsigned long value_size = hcsp_data.value_size;

    TCPPacket res_packet;
    switch(cmd) {
        case HCSP_CMD_INSERT: {
            int status = hashtable_insert(ht, key, value, value_size);
            hcsp_pack(&res_packet, NULL, status);
            break;
        }
        case HCSP_CMD_READ: {
            Value *v = hashtable_get(ht, key);
            int status = HCSP_STATUS_KEY_NOT_FOUND_ERR;
            if (v) {
                status = HCSP_STATUS_SUCCESS;
            } 
            hcsp_pack(&res_packet, v, status);
            break;
        }
        case HCSP_CMD_UPDATE: {
            int status = hashtable_update(ht, key, value, value_size);
            hcsp_pack(&res_packet, NULL, status);
            break;
        }
        case HCSP_CMD_DELETE: {
            int status = hashtable_delete(ht, key);
            hcsp_pack(&res_packet, NULL, status);
            break;
        }
    }
    return res_packet;
}