#ifndef CAN_H
#define CAN_H

#include "Communication/CANMessage.h"
#include "Libraries/FlexCAN/FlexCAN.h"

#define STEROIDO_STD_CAN_BAUD (uint16_t)250000
#define STEROIDO_STD_CAN_MESSAGE_TIMEOUT 10

class CAN : private NonCopyable<CAN> {
    public:
        // Construct FlexCAN based CAN Object. rd and td are only given for compatibility reasons, starndard Pins of Teensy will be used!
        CAN(PinName rd, PinName td, uint16_t hz = STEROIDO_STD_CAN_BAUD)
        : _can(hz) {
            _can.begin();
        }

        virtual ~CAN() {
            _can.end();
        }

        // Send a frame of up to 8 bytes. write() will return 0 if no buffer was available for sending.
        uint8_t write(CANMessage &msg) {
            // Convert CANMessage to FlexCAN Message
            CAN_message_t flexMessage;
            
            flexMessage.id = msg.id;

            if (msg.format == CANStandard) {
                flexMessage.ext = 0;
            } else {
                flexMessage.ext = 1;
            }

            flexMessage.len = msg.len;
            flexMessage.timeout = STEROIDO_STD_CAN_MESSAGE_TIMEOUT;

            for (uint8_t i = 0; i < msg.len; i++) {
                flexMessage.buf[i] = msg.data[i];
            }

            return _can.write(flexMessage);
        }

        // Receive a frame into "message" if available. read() will return 1 if a frame was copied into the callers buffer, or 0 if no frame is available
        uint8_t read(CANMessage &msg) {
            CAN_message_t flexMessage;
            uint8_t returnValue = _can.read(flexMessage);

            if (returnValue == 0) return 0;

            msg.id = flexMessage.id;
            
            if (flexMessage.ext == 0) {
                msg.format = CANStandard;
            } else {
                msg.format = CANExtended;
            }

            msg.len = flexMessage.len;

            for (uint8_t i = 0; i < flexMessage.len; i++) {
                msg.data[i] = flexMessage.buf[i];
            }

            return returnValue;
        }

        // Returns 1 if at least one receive frame is waiting, or 0 if no frame is available.
        uint8_t available() {
            return _can.available();
        }
        
        void reset() {}

        uint8_t rderror() {
            // No implementation needed
            return 0;
        }

        uint8_t tderror() {
            // No implementation needed
            return 0;
        }
    
    protected:
        FlexCAN _can;
};

#endif // CAN_H