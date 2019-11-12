#ifndef CANMESSAGE_H
#define CANMESSAGE_H

enum CANFormat {
    CANStandard = 0,
    CANExtended = 1,
    CANAny = 2
};
typedef enum CANFormat CANFormat;

enum CANType {
    CANData   = 0,
    CANRemote = 1
};
typedef enum CANType CANType;

struct CAN_Message {
    uint32_t   id;       // 29 bit identifier
    uint8_t    data[8];  // Data field
    uint8_t    len;      // Length of data field in bytes
    CANFormat  format;   // Format ::CANFormat
    CANType    type;     // Type ::CANType
};
typedef struct CAN_Message CAN_Message;

class CANMessage : public CAN_Message {
    public:
        CANMessage() : CAN_Message() {
            len    = 8;
            type   = CANData;
            format = CANStandard;
            id     = 0;
            memset(data, 0, 8);
        }

        CANMessage(uint32_t _id, const uint8_t *_data, uint8_t _len, CANType _type = CANData, CANFormat _format = CANStandard) {
            len = _len & 0xF;
            type = _type;
            format = _format;
            id = _id;
            memcpy(data, _data, _len);
        }

        CANMessage(uint32_t _id, CANFormat _format = CANStandard) {
            len    = 0;
            type   = CANRemote;
            format = _format;
            id     = _id;
            memset(data, 0, 8);
        }
};

#endif // CANMESSAGE_H