# mcu-interface

MCU interface for communicating instruction from Raspberry Pi to low level hardware.

# Pinout Configuration

- Connectivity
  - USART2
    - Mode: Asynchronous



## TODO

- [ ] Enable i2c1
  - PB8 -> I2C1_SCL
  - PB9 -> I2C1_SDA
  - Others remain default
- [ ] TIM4_Channel1 -> PWM Generation
  - PRE = 1679
  - ARR = 1000
  - Others remain default
- [ ] Complete UART communication