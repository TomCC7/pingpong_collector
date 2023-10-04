/*
 * MotorShield.cpp
 *
 *      Author: WHX
 */

#include "MotorShield.h"

MotorShield::MotorShield(I2C_HandleTypeDef *hi2c1, UART_HandleTypeDef *huart2,
                         uint8_t addr)
/**
 * @brief  MotorShield class Constructor
 * @param  hi2c1: pointer to I2C_HandleTypeDef
 *             In this project, pass in &hi2c1
 * @param  huart2: pointer to UART_HandleTypeDef for debug output
 *             pass in &huart2
 * @param  addr: I2C device address
 *             pass in MS_ADDR
 */
{
    _i2c_addr = addr;
    _i2c = hi2c1;
    _uart = huart2;

    Reset();
    SetPwmFreq(PWM_FREQ);
    for (uint8_t i = 0; i < 16; i++)
    {
        SetPwm(i, 0, 0);
    }

    // uint8_t msg[40];
    // sprintf((char *)msg, "Initializing MotorShield at %#04x...\r\n", addr);
    // HAL_UART_Transmit(_uart, msg, sizeof(msg), 10000);
    Motors[1] = {8, 9, 10};
    Motors[3] = {13, 12, 11};
    Motors[2] = {2, 3, 4};
    Motors[0] = {7, 6, 5};
}

uint8_t MotorShield::Read8(uint8_t addr)
/**
 * @brief  Read one byte from sub-address
 * @param  addr: target sub-address
 */
{
    uint8_t buffer[1] = {addr};
    // write (buffer, write_len, stop=false)
    HAL_I2C_Master_Transmit(_i2c, _i2c_addr, buffer, 1, 10);

    // read (buffer, read_len)
    HAL_I2C_Master_Receive(_i2c, _i2c_addr, buffer, 1, 10);
    return buffer[0];
}

void MotorShield::Write8(uint8_t addr, uint8_t d)
/**
 * @brief  Write one byte to sub-address
 * @param  addr: target sub-address
 */
{
    uint8_t buffer[2] = {addr, d};

    HAL_I2C_Master_Transmit(_i2c, _i2c_addr, buffer, 2, 10);
}

void MotorShield::Reset()
{
    Write8(PCA9685_MODE1, 0x00);
}

void MotorShield::SetPwmFreq(uint16_t Frequency)
/**
 * @brief  Set the PWM frequency for the PWM generator on Motor Shield
 * @param  Frequency: frequency to set
 */
{
    Frequency *= 0.9;
    float prescaleval = 25000000;
    prescaleval /= 4096;
    prescaleval /= Frequency;
    prescaleval -= 1;
    uint8_t prescale = floor(prescaleval + 0.5);
    uint8_t oldmode = Read8(PCA9685_MODE1);    // 0X00
    uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
    Write8(PCA9685_MODE1, newmode);            // go to sleep
    Write8(PCA9685_PRESCALE, prescale);        // set the prescaler
    Write8(PCA9685_MODE1, oldmode);
    HAL_Delay(5);
    Write8(
        PCA9685_MODE1,
        oldmode |
            0xa1); //  This sets the MODE1 register to turn on auto increment.
}

void MotorShield::SetPwm(uint8_t Channel, uint16_t OnTime, uint16_t OffTime)
/**
 * @brief  Helper function: Set PWM duty cycle for a specific pin
 */
{
    uint8_t buffer[5];
    buffer[0] = PCA9685_LED0_ON_L + 4 * Channel;
    buffer[1] = OnTime;
    buffer[2] = OnTime >> 8;
    buffer[3] = OffTime;
    buffer[4] = OffTime >> 8;
    HAL_I2C_Master_Transmit(_i2c, _i2c_addr, buffer, 5, 10);
}

void MotorShield::SetPin(uint8_t Channel, uint16_t Value)
/**
 * @brief  Set PWM duty cycle for a specific pin
 * @param  Channel: target channel
 * @param  Value: [0-4096]
 */
{
    if (Value > 4095)
    {
        SetPwm(Channel, 4096, 0);
    }
    else
    {
        SetPwm(Channel, 0, Value);
    }
}

void MotorShield::SetMotorDirection(uint8_t MotorID, uint8_t cmd)
/**
 * @brief  Set direction for a specific motor
 * @param  MotorID: target motor ID, [0-3]
 * @param  cmd: FORWARD, BACKWARD, or RELEASE
 */
{
    switch (cmd)
    {
    case FORWARD:
        SetPin(Motors[MotorID].in1, 0);
        SetPin(Motors[MotorID].in2, 4096);
        break;
    case BACKWARD:
        SetPin(Motors[MotorID].in2, 0);
        SetPin(Motors[MotorID].in1, 4096);
        break;
    case RELEASE:
        SetPin(Motors[MotorID].in1, 0);
        SetPin(Motors[MotorID].in2, 0);
        break;
    }
}

void MotorShield::SetMotorSpeed(uint8_t MotorID, uint8_t speed)
/**
 * @brief  Set speed for a specific motor
 * @param  MotorID: target motor ID, [0-3]
 * @param  speed: target speed, [0-255]
 */
{
    SetPin(Motors[MotorID].pwm, speed * 16);
}

void MotorShield::SetDirection(uint8_t cmd)
/**
 * @brief  Set direction for all motors
 * @param  cmd: FORWARD, BACKWARD, or RELEASE
 */
{
    for (uint8_t i = 0; i < 4; i++)
    {
        SetMotorDirection(i, cmd);
    }
}

void MotorShield::SetSpeed(uint8_t speed)
/**
 * @brief  Set speed for a all motors
 * @param  speed: target speed, [0-255]
 */
{
    for (uint8_t i = 0; i < 4; i++)
    {
        SetMotorSpeed(i, speed);
    }
}

void MotorShield::SetMotor(uint8_t MotorID, uint8_t pwm, uint8_t in1,
                           uint8_t in2)
/**
 * @brief  Set params for a specific motor
 * @param  MotorID: target motor ID, [0-3]
 * @param  pwm: speed, [0-255]
 * @param  in1: HIGH, or LOW
 * @param  in2: HIGH, or LOW
 */
{
    SetPin(Motors[MotorID].pwm, pwm * 16);
    if (in1 == HIGH && in2 == LOW){
        SetPin(Motors[MotorID].in2, 0);
        SetPin(Motors[MotorID].in1, 4096);
    }
    else if (in1 == LOW && in2 == HIGH){
        SetPin(Motors[MotorID].in1, 0);
        SetPin(Motors[MotorID].in2, 4096);
    }
    else if (in1 == LOW){
        SetPin(Motors[MotorID].in1, 0);
        SetPin(Motors[MotorID].in2, 0);
    }
    else if (in1 == HIGH){
        SetPin(Motors[MotorID].in1, 4096);
        SetPin(Motors[MotorID].in2, 4096);
    }

    // if (in1 == HIGH)
    //     SetPin(Motors[MotorID].in1, 4096);
    // else
    //     SetPin(Motors[MotorID].in1, 0);
    // if (in2 == HIGH)
    //     SetPin(Motors[MotorID].in2, 4096);
    // else
    //     SetPin(Motors[MotorID].in2, 0);
}

void MotorShield::go_dir(int8_t x, int8_t y)
{
    int32_t base_val_straight[] = {-1,-1,-1,-1};
    int32_t base_val_cross[] = {-1,1,1,-1};
    int32_t output[4];
    for (int i = 0; i < 4; i++)
    {
        output[i] = (base_val_straight[i] * (int32_t)x + base_val_cross[i] * (int32_t)y);
        uint8_t in1 = (output[i] >= 0);
        SetMotor(i, (uint8_t)abs(output[i]), in1, !in1);
    }
}

void MotorShield::rotate(bool dir)
{
    int i = 0;
    for (;i < 2;i++)
        SetMotor(i, 150, dir, !dir);
    for (;i<4;i++)
        SetMotor(i, 150, !dir, dir);
}
