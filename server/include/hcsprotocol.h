#ifndef HCSPROTOCOL_H
#define HCSPROTOCOL_H

#define HCSP_CMD_READ                   1
#define HCSP_CMD_INSERT                 2
#define HCSP_CMD_UPDATE                 3
#define HCSP_CMD_DELETE                 4

#define HCSP_STATUS_SUCCESS             HT_STATUS_OK
#define HCSP_STATUS_KEY_EXIST_ERR       HT_STATUS_KEY_EXIST_ERR
#define HCSP_STATUS_KEY_NOT_FOUND_ERR   HT_STATUS_KEY_NOT_FOUND_ERR
#define HCSP_STATUS_UNKNOWN_CMD         50

#define HCSP_DATA_SIZE                  244
#define HCSP_DATA_CMD_TYPE              char
#define HCSP_DATA_KEY_TYPE              char
#define HCSP_DATA_KEY_SIZE              50
#define HCSP_DATA_VALUE_SIZE_TYPE       int
#define HCSP_DATA_VALUE_TYPE            char
#define HCSP_DATA_STATUS_TYPE           char
#define HCSP_DATA_VALUE_SIZE                                    \
        (HCSP_DATA_SIZE                                     -   \
        sizeof(HCSP_DATA_CMD_TYPE)                          -   \
        (sizeof(HCSP_DATA_KEY_TYPE) * HCSP_DATA_KEY_SIZE)   -   \
        sizeof(HCSP_DATA_VALUE_SIZE_TYPE)                   -   \
        sizeof(HCSP_DATA_STATUS_TYPE))                      /   \
        sizeof(HCSP_DATA_VALUE_TYPE)

struct TCPPacket;
struct Value;
struct HashTable;

#pragma pack(push, 1)
typedef struct HCSPData {
    HCSP_DATA_CMD_TYPE          cmd;
    HCSP_DATA_KEY_TYPE          key [HCSP_DATA_KEY_SIZE];
    HCSP_DATA_VALUE_SIZE_TYPE   value_size;
    HCSP_DATA_VALUE_TYPE        value [HCSP_DATA_VALUE_SIZE];
    HCSP_DATA_STATUS_TYPE       status;
} HCSPData;
#pragma pack(pop)

void hcsp_unpack(HCSPData *dest, struct TCPPacket *packet);
void hcsp_pack(struct TCPPacket *dest, struct Value *value, char status);
struct TCPPacket hcsp_handle_packet(struct HashTable *ht, struct TCPPacket *packet);

#endif // HCSPROTOCOL_H