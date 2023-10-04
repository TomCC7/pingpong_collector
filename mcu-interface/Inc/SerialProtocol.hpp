#ifndef __SerialProtocol_h__
#define __SerialProtocol_h__

#include "usart.h"
#include <cstring>

// 2 0xFF to initiate a transaction

// Header (1B)
//   device code (4b) 0: screen, 1: motor, 2: servo, other reserved
//   sub device code (4b): for things like index of motor or servo
//   index of the motors are defined as
//                           1 ---- 0
//                           |  ->  |
//                           3 ---- 2
//
// Data Section
// bytes specified in packet size
// for example, a motor needs 2 bytes: 1 byte pwm + 2 bit control
// a servo needs 1 byte as pwm
// a screen needs 2 bytes,
//        the first byte indicates the number of balls detected
//        the second byte indicates the number of balls collected
// 2 0xFF to end a transaction

// enforced small endian

typedef enum __Device : uint8_t
{
    SCREEN,
    MOTOR,
    SERVO
} Device;

typedef struct __Header
{
    uint8_t index : 4;
    uint8_t device : 4;
} Header;

typedef struct __MotorData
{
    uint8_t pwm;
    uint8_t in1 : 1;
    uint8_t in2 : 1;
    uint8_t reserved : 6;
} MotorData;

template <size_t BUFFER_SIZE>
class Packet
{
public:
    Header header;
    uint8_t data[BUFFER_SIZE + 2];
    uint8_t size;
};

// initialization of serial port is done outside
class SerialTransceiver
{
protected:
    UART_HandleTypeDef &port;

public:
    static constexpr uint8_t terminatorByte = 0xFF;
    uint8_t terminator[2] = {terminatorByte, terminatorByte};

    SerialTransceiver(UART_HandleTypeDef &_port) : port(_port)
    {
    }

    template <size_t BUFFER_SIZE>
    HAL_StatusTypeDef receive(Packet<BUFFER_SIZE> &packet)
    {
        uint8_t terminator_dump = 0;
        do
        {
            HAL_StatusTypeDef _ =
                HAL_UART_Receive(&port, &terminator_dump, 1, HAL_MAX_DELAY);
            UNUSED(_);
        } while (terminator_dump != terminatorByte);
        do
        {
            HAL_StatusTypeDef _ =
                HAL_UART_Receive(&port, &terminator_dump, 1, HAL_MAX_DELAY);
            UNUSED(_);
        } while (terminator_dump == terminatorByte);
        uint8_t index = terminator_dump & 0xF;
        uint8_t device = terminator_dump >> 4;
        packet.header = Header{index, device};
        HAL_StatusTypeDef status = HAL_OK;
        status = HAL_UART_Receive(&port, packet.data, 1, HAL_MAX_DELAY);
        packet.size++;
        uint8_t previous_terminator = 0;
        while (packet.size < BUFFER_SIZE + 2)
        {
            status = HAL_UART_Receive(&port, packet.data + packet.size, 1,
                                      HAL_MAX_DELAY);
            uint8_t is_terminator = packet.data[packet.size] == terminatorByte;
            if (is_terminator && previous_terminator)
            {
                packet.size--;
                break;
            }
            previous_terminator = is_terminator;
            packet.size++;
        }
        return status;
    }

    template <size_t BUFFER_SIZE>
    HAL_StatusTypeDef transmit(Packet<BUFFER_SIZE> &packet)
    {
        HAL_StatusTypeDef status =
            HAL_UART_Transmit(&port, this->terminator, 2, HAL_MAX_DELAY);
        if (status != HAL_OK)
        {
            return status;
        }
        status =
            HAL_UART_Transmit(&port, packet.data, packet.size, HAL_MAX_DELAY);
        if (status != HAL_OK)
        {
            return status;
        }
        status = HAL_UART_Transmit(&port, this->terminator, 2, HAL_MAX_DELAY);
        return status;
    }

    HAL_StatusTypeDef heartbeat(uint8_t c = terminatorByte)
    {
        return HAL_UART_Transmit(&port, &c, 1, HAL_MAX_DELAY);
    }
};

#endif
