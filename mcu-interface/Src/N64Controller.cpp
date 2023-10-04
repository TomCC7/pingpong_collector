#include "N64Controller.h"

N64::N64() : N64handle(GPIOA, 9), is_connected(false)
{
    N64handle.start();
    check_connection();
}

void N64::check_connection()
{
    N64handle.send(N64_DETECT);
    uint8_t data = N64handle.receive();
    if (data == 0x05) is_connected = true;
}

N64Packet N64::query()
{
    N64Packet result;
    N64handle.send(N64_POLL);
    N64handle.receive(&result, sizeof(result));
    return result;
}
