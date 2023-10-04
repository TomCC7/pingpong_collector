#ifndef __IOStream_h__
#define __IOStream_h__

#include <cstdint>

class IOStream
{
public:
    virtual int send(uint8_t ch) = 0;
    virtual int receive(uint8_t &ch) = 0;
    virtual int send(const uint8_t *data, std::size_t length) = 0;
    virtual int receive(uint8_t *data, std::size_t length) = 0;
};

#endif