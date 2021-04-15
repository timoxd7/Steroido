#ifndef CANMESSAGE_H
#define CANMESSAGE_H

/**
 * @brief The Format of a CAN Message
 * 
 */
enum CANFormat {
    CANStandard = 0,
    CANExtended = 1,
    CANAny = 2
};
typedef enum CANFormat CANFormat;

/**
 * @brief The Type of the CAN Message
 * 
 */
enum CANType {
    CANData   = 0,
    CANRemote = 1
};
typedef enum CANType CANType;

/**
 * @brief A CAN Message struct for the data saved in a CAN Message
 * 
 */
struct CAN_Message {
    uint32_t   id;       // 29 bit identifier
    uint8_t    data[8];  // Data field
    uint8_t    len;      // Length of data field in bytes
    CANFormat  format;   // Format ::CANFormat
    CANType    type;     // Type ::CANType
};
typedef struct CAN_Message CAN_Message;

/**
 * @brief A CAN Message Object as it gets send/received over a CAN Bus
 * 
 */
class CANMessage : public CAN_Message {
    public:
        /**
         * @brief Construct a new empty CANMessage with Standard ID
         * 
         */
        CANMessage() : CAN_Message() {
            len    = 8;
            type   = CANData;
            format = CANStandard;
            id     = 0;
            memSet<uint8_t>(data, 0, 8);
        }

        /**
         * @brief Construct a new CANMessage object
         * 
         * @param _id 
         * @param _data 
         * @param _len 
         * @param _type 
         * @param _format 
         */
        CANMessage(uint32_t _id, const uint8_t *_data, uint8_t _len, CANType _type = CANData, CANFormat _format = CANStandard) {
            len = _len & 0xF;
            type = _type;
            format = _format;
            id = _id;
            memCpy<uint8_t>(data, _data, _len);
        }

        /**
         * @brief Construct a new CANMessage object
         * 
         * @param _id 
         * @param _format 
         */
        CANMessage(uint32_t _id, CANFormat _format = CANStandard) {
            len    = 0;
            type   = CANRemote;
            format = _format;
            id     = _id;
            memSet<uint8_t>(data, 0, 8);
        }
};

#endif // CANMESSAGE_H