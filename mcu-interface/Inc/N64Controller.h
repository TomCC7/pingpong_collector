#ifndef __N64Controller_h__
#define __N64Controller_h__

#include "OneWire.h"
#include <cstdint>

typedef enum __N64Command : uint8_t
{
    N64_DETECT = 0x00,
    N64_POLL = 0x01,
    N64_READ = 0x02,
    N64_WRITE = 0x03,
    N64_RESET = 0xFF
} N64Command;

typedef struct __N64Packet
{
    uint8_t DR : 1;
    uint8_t DL : 1;
    uint8_t DD : 1;
    uint8_t DU : 1;
    uint8_t START : 1;
    uint8_t Z : 1;
    uint8_t B : 1;
    uint8_t A : 1;

    uint8_t CR : 1;
    uint8_t CL : 1;
    uint8_t CD : 1;
    uint8_t CU : 1;
    uint8_t R : 1;
    uint8_t L : 1;
    uint8_t RESERVED : 1;
    uint8_t RESET : 1;

    int8_t JoystickX;
    int8_t JoystickY;
} N64Packet;

class N64
{
  private:
    OneWire N64handle;
    /**
     * @brief check whether N64 is connected
     **/
    void check_connection();

  public:
    N64();
    bool is_connected;

    /**
     * @brief  query the current state
     * @return N64Packet
     **/
    N64Packet query();
};

#endif
