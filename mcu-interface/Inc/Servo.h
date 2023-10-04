/*
 * Servo.hpp
 *
 *      Author: WHX
 */

#ifndef INC_SERVO_HPP_
#define INC_SERVO_HPP_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"

#define SERVO_UPPER 250
#define SERVO_LOWER 40

class Servo{
public:
    Servo(TIM_HandleTypeDef *htim);
    void SetServoAngle(uint8_t Angle);

private:
    TIM_HandleTypeDef *_htim;
};



#endif /* INC_SERVO_HPP_ */
