/*
 * MotorShield.hpp
 *
 *      Author: WHX
 */

#ifndef INC_MOTORSHIELD_HPP_
#define INC_MOTORSHIELD_HPP_

#include "stm32f4xx_hal.h"
#include "SerialProtocol.hpp"
#include <stdio.h>
#include <math.h>

#define LOW 0
#define HIGH 1

// Servo min and max value
#define SERVO_MIN 110
#define SERVO_MAX 500
#define MIN_ANGLE 0.0
#define MAX_ANGLE 180.0

// Motor Def
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

// I2C
#define PWM_FREQ 1600
#define MS_ADDR 0x60 << 1

#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define PCA9685_LED0_ON_L 0x6
#define PCA9685_LED0_ON_H 0x7
#define PCA9685_LED0_OFF_L 0x8
#define PCA9685_LED0_OFF_H 0x9
#define PCA9685_ALLLED_ON_L 0xFA
#define PCA9685_ALLLED_ON_H 0xFB
#define PCA9685_ALLLED_OFF_L 0xFC
#define PCA9685_ALLLED_OFF_H 0xFD

#define PCA9685_MODE1_SLEEP_BIT 4
#define PCA9685_MODE1_AI_BIT 5
#define PCA9685_MODE1_EXTCLK_BIT 6
#define PCA9685_MODE1_RESTART_BIT 7

struct Motor_t
{
  uint8_t pwm;
  uint8_t in1;
  uint8_t in2;
};

class MotorShield
{
public:
  MotorShield(I2C_HandleTypeDef *hi2c1, UART_HandleTypeDef *huart2, uint8_t addr);
  void SetDirection(uint8_t cmd);
  void SetSpeed(uint8_t speed);
  void SetMotor(uint8_t MotorID, uint8_t pwm, uint8_t in1, uint8_t in2);
  void go_dir(int8_t x, int8_t y);
  void rotate(bool dir);
  void SetMotorDirection(uint8_t MotorID, uint8_t cmd);
  void SetMotorSpeed(uint8_t MotorID, uint8_t speed);

private:
  I2C_HandleTypeDef *_i2c;
  UART_HandleTypeDef *_uart;
  Motor_t Motors[4];
  uint8_t _i2c_addr;

  uint8_t Read8(uint8_t addr);
  void Write8(uint8_t addr, uint8_t d);
  void Reset();
  void SetPwmFreq(uint16_t Frequency);
  void SetPwm(uint8_t Channel, uint16_t OnTime, uint16_t OffTime);
  void SetPin(uint8_t Channel, uint16_t Value);
};

#endif /* INC_MOTORSHIELD_HPP_ */
