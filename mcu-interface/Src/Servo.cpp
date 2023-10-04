/*
 * Servo.cpp
 *
 *      Author: WHX
 */

#include "Servo.h"

Servo::Servo(TIM_HandleTypeDef *htim)
/**
 * @brief  Servo class Constructor
 * @param  htim: pointer to TIM_HandleTypeDef
 *             In this project, pass in &htim4
 */
{
  _htim = htim;
  HAL_TIM_PWM_Start(_htim, TIM_CHANNEL_1);
  __HAL_TIM_SET_COMPARE(_htim, TIM_CHANNEL_1, 150); // set to middle
}

void Servo::SetServoAngle(uint8_t Angle)
/**
 * @param  Angle: target servo angle, [0,255]
 */
{
  float Value = (float)SERVO_LOWER + (float)Angle / 255.0 * ((float)SERVO_UPPER - (float)SERVO_LOWER);

  __HAL_TIM_SET_COMPARE(_htim, TIM_CHANNEL_1, (uint16_t)Value);
}
