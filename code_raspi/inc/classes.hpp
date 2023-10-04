#ifndef CLASSES_H_
#define CLASSES_H_
#include "camera.hpp"
#include <opencv2/core.hpp>

class Ball3D;

/** @brief 2D ball instance class */
class Ball2D
{
  public:
    // center of the ball in the frame
    cv::Point2f center;
    // size of the ball in the frame
    double radius;

    /**
     * @brief constructor of Ball2D
     * @param center: cv::Point2f indicating the center of the ball in the frame
     * @param radius: radius of the ball in pixels
     **/
    Ball2D(cv::Point2f center, double radius);

    /**
     * @brief draws the ball on the frame
     * @param frame: frame to be drawn
     * @param color: rgb color to draw the circle
     * @param thickness: line thickness for the circle
     **/
    void draw(cv::Mat &frame, cv::Scalar color = cv::Scalar(0, 255, 255),
              int thickness = 2);

    /**
     * @brief convert current 2D ball to 3D using the camera coordinate
     **/
    Ball3D to_3D();
};

/** @brief 3D ball instance class */
class Ball3D
{
  public:
    // center of the ball in cam coordinate
    cv::Point3d center;
    double distance;
    Ball2D ball_2d;
    Ball3D(cv::Point3d center, double distance, const Ball2D &ball);
    Ball3D(cv::Point3d center, double distance);

    /**
     * @brief draw the ball on the frame, with distance
     **/
    void draw(cv::Mat &frame, cv::Scalar color = cv::Scalar(0, 255, 255),
              int thickness = 2);
};

/**
 * @brief robot class
 **/
class Bot
{
  private:
    // location of the bot, initialized to be 0
    cv::Point3d loc;

  public:
    Bot();
};

void test_to_3D();

#endif // BALL_H_
