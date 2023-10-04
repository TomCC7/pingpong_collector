#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <cmath>
#include <opencv2/opencv.hpp>
#include <string>

namespace Settings
{
const cv::Scalar COLOR_LOWER(5, 127, 127);
const cv::Scalar COLOR_UPPER(25, 255, 255);
const int MORPH_SIZE = 5;
// threshold for the pingpong ball radius, value in pixel
const int MIN_BALL_R = 12;
// minimum "visible" area of the ball
const double MIN_VISIBLE_RATIO = 0.3;
// radius of the ball in mm
const double BALL_RADIUS = 20.1;
// location of the serial device
const std::string SERIAL_PORT = "/dev/ttyACM0";
// speed settings
const uint8_t ROT_SPEED = 100; // rotation settings
const uint8_t SPEED = 200; // max speed of the car
// ball center thresh
const uint CENTER_THRESH = 50;
// camera index
const uint8_t CAM_ID = 0;
} // namespace Settings

#endif // SETTINGS_H_
