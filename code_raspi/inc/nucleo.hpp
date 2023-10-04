#ifndef NUCLEO_H_
#define NUCLEO_H_
#include "serial.hpp"
#include <opencv2/core.hpp>

/**
 * @brief class to control the nucleo board slave device
 * The communication protocol defined is:
 *   2 0xFF to initiate a transaction
 *
 *   Header (1B)
 *     device code (4b) 0: screen, 1: motor, 2: servo, other reserved
 *     sub device code (4b): for things like index of motor or servo
 *     index of the motors are defined as
 *                             1 ---- 0
 *                             |  ->  |
 *                             3 ---- 2
 *
 *   Data Section
 *   bytes specified in packet size
 *   for example, a motor needs 2 bytes: 1 byte pwm + 2 bit control
 *   a servo needs 1 byte as pwm
 *   a screen needs 2 bytes,
 *          the first byte indicates the number of balls detected
 *          the second byte indicates the number of balls collected
 *   2 0xFF to end a transaction
 **/
class Nucleo
{
  public:
    /**
     * @brief default constructor
     * @param serial_port: location of the serial device
     * @return Nucleo instance
     **/
    Nucleo(string serial_port);

    /**
     * @brief reset motors */
    ~Nucleo();

    /**
     * @brief set the servo status
     * @param pos: pwm value to control position
     * @return TODO: need test
     **/
    int set_servo(uint8_t pos);

    /**
     * @brief send the statistics of the balls to display on screen
     * @param num_detected: number of balls detected but not collected
     * @param num_collected: number of balls already collected
     * @return TODO: need test
     **/
    int send_ball_numbers(uint8_t num_detected, uint8_t num_collected);

    /**
     * @brief set the car to go straight
     * @param speed: pwm value of the motor
     * @param inverse: whether to go backward
     **/
    void go_straight(uint8_t speed, bool inverse = false);

    /**
     * @brief set the car to rotate
     * @param speed: pwm value of rotation
     * @param direction: bool value indicate rotate direction
     *                   0 as left, 1 as right
     **/
    void rotate(uint8_t speed, bool direction);

    /**
     * @brief set the status of a single motor
     * @param index: index of the motor defined as
     *                1 ---- 0
     *                |  ->  |
     *                3 ---- 2
     * @param speed: pwm value to control speed
     * @param inverse or not
     * @return TODO: need test
     **/
    int set_motor(uint8_t index, uint8_t speed, bool inverse = false);

  private:
    /**
     * @param serial_port  */
    Serial dev;

};

void test_nucleo();

#endif // NUCLEO_H_
