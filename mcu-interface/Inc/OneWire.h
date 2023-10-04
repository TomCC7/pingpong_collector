#ifndef __OneWire_h__
#define __OneWire_h__

#include "CriticalSection.hpp"
#include "gpio.h"
#include <cstdint>

class OneWire
{
  protected:
    GPIO_TypeDef *port;
    GPIO_InitTypeDef state;
    uint8_t pin;
    uint8_t buffer[33];

    inline void __send_bit(GPIO_PinState bit);
    inline void __send_stop(void);
    inline uint8_t __receive_bit(void);
    inline void __send(uint8_t byte);
    inline uint8_t __receive(void);
    inline void __input(void);

  public:
    OneWire(GPIO_TypeDef *_port, uint8_t _pin);

    void start(void);

    void output(void);

    void input(void);

    void send(uint8_t byte);
    uint8_t receive(void);
    void send(const void *data, size_t length);
    void receive(void *data, size_t length);
};

#endif