#include "OneWire.h"

OneWire::OneWire(GPIO_TypeDef *_port, uint8_t _pin)
    : port(_port), state{0}, pin(_pin)
{
    state.Pin = 0b1 << pin;
    state.Mode = GPIO_MODE_OUTPUT_OD;
    state.Pull = GPIO_NOPULL;
    state.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
}

inline void OneWire::__send_bit(GPIO_PinState bit)
{
    HAL_GPIO_WritePin(port, state.Pin, GPIO_PIN_RESET);
    CriticalSection::__delay_us(1);
    HAL_GPIO_WritePin(port, state.Pin, bit);
    CriticalSection::__delay_us(2);
    HAL_GPIO_WritePin(port, state.Pin, GPIO_PIN_SET);
    CriticalSection::__delay_us(1);
}

inline void OneWire::__send_stop(void)
{
    HAL_GPIO_WritePin(port, state.Pin, GPIO_PIN_RESET);
    CriticalSection::__delay_us(1);
    HAL_GPIO_WritePin(port, state.Pin, GPIO_PIN_SET);
}

inline uint8_t OneWire::__receive_bit(void)
{
    while (HAL_GPIO_ReadPin(port, state.Pin))
        ;
    CriticalSection::__delay_us(2);
    uint8_t result = (uint8_t)(HAL_GPIO_ReadPin(port, state.Pin));
    while (!HAL_GPIO_ReadPin(port, state.Pin))
        ;
    return result;
}

inline void OneWire::__send(uint8_t byte)
{
    uint8_t mask = 0b10000000;
    do
    {
        __send_bit((GPIO_PinState)(byte & mask));
    } while (mask >>= 1);
}

inline uint8_t OneWire::__receive(void)
{
    uint8_t result = 0;
    for (int8_t i = 7; i >= 0; --i)
    {
        result |= __receive_bit() << i;
    }
    return result;
}

inline void OneWire::__input(void)
{
    state.Mode = GPIO_MODE_INPUT;
    uint32_t cache = port->MODER;
    cache &= ~(GPIO_MODER_MODER0 << (pin << 1));
    cache |= ((state.Mode & GPIO_MODE) << (pin << 1));
    port->MODER = cache;
}

void OneWire::start(void)
{
    output();
    CriticalSection::__update_delay_coefficient();
}

void OneWire::output(void)
{
    HAL_GPIO_WritePin(port, state.Pin, GPIO_PIN_SET);
    state.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(port, &state);
}

void OneWire::input(void)
{
    CriticalSection lock;
    UNUSED(lock);
    __input();
}

void OneWire::send(uint8_t byte)
{
    output();
    CriticalSection lock;
    UNUSED(lock);
    __send(byte);
    __send_stop();
}

uint8_t OneWire::receive(void)
{
    CriticalSection lock;
    UNUSED(lock);
    __input();
    return __receive();
}

void OneWire::send(const void *data, size_t length)
{
    output();
    CriticalSection lock;
    UNUSED(lock);
    for (size_t i = 0; i < length; i++)
    {
        __send(((const uint8_t *)data)[i]);
    }
    __send_stop();
}

void OneWire::receive(void *data, size_t length)
{
    CriticalSection lock;
    UNUSED(lock);
    input();
    for (size_t i = 0; i < length; i++)
    {
        ((uint8_t *)data)[i] = __receive();
    }
}