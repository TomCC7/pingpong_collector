#include "nucleo.hpp"
#include "settings.hpp"
#include <iostream>

Nucleo::Nucleo(string serial_port)
    : dev(Serial(serial_port, B115200, CS8, false))
{
    std::cout << "Nucleo connected" << std::endl;
}

Nucleo::~Nucleo()
{
    // stop motor
    this->go_straight(0);
}

int Nucleo::set_motor(uint8_t index, uint8_t pwm, bool inverse)
{
    // NOTE: sleeep to avoid problem
    usleep(2000);
    // header specify device to be motor and set the index
    uint8_t header = (index & 0xF) | (0x1 << 4);
    uint8_t mode = inverse ? 0b01 : 0b10;
    int ret =
        dev.send(vector<uint8_t>({0xFF, 0xFF, header, pwm, mode, 0xFF, 0xFF}));
    auto rec = dev.receive(1);
    assert(rec[0] == 0xFF);
    return ret;
}

int Nucleo::set_servo(uint8_t pwm)
{
    uint8_t header = 0x2 << 4; // header specify device to be servo
    // TODO: change this
    uint8_t mode = 0b01;
    int ret = dev.send(vector<uint8_t>({0xFF, 0xFF, header, pwm, 0xFF, 0xFF}));
    auto rec = dev.receive(1);
    assert(rec[0] == 0xFF);
    return ret;
}

int Nucleo::send_ball_numbers(uint8_t num_detected, uint8_t num_collected)
{
    uint8_t header = 0x0; // this specify device to be screen
    int ret = dev.send(vector<uint8_t>(
        {0xFF, 0xFF, header, num_detected, num_collected, 0xFF, 0xFF}));
    auto rec = dev.receive(1);
    assert(rec[0] == 0xFF);
    return ret;
}

void Nucleo::go_straight(uint8_t speed, bool inverse)
{
    for (int i = 0; i < 4; i++)
    {
        int ret = this->set_motor(i, speed, inverse);
    }
}

void Nucleo::rotate(uint8_t speed, bool direction)
{
    for (int i = 0; i < 4; i++)
    {
        // inverse = (i<2) ? !direction : direction
        this->set_motor(i, speed, (i < 2) ^ direction);
    }
}

void test_nucleo()
{
    Nucleo n(Settings::SERIAL_PORT);
    std::cout << "start testing nucleo" << std::endl;
    while (true)
    {
        n.go_straight(100);
        sleep(2);
        n.go_straight(0);
        sleep(2);
    }
}
