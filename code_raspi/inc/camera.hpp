#ifndef CAMERA_H_
#define CAMERA_H_

#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

/**
 * @brief wrapper of raspberrypi camera rev1.3 we use
 *        singleton class
 **/
class Cam
{
  public:
    // width and height of the frame in pixels
    const int pwidth = 640;
    const int pheight = 480;
    // field of view in radian
    const double field_of_view = 0.35 * M_PI;
    // focal length in mm
    const double flength = 35.0;
    // focal length in pixel
    const double fl_pixel = (double)(pwidth) / (2 * tan(field_of_view / 2));

    /** @brief get the instance of the singleton element
     *  @static
     *  @param index: index to be passed to cv::VideoCapture
     *                will be ignored after instance is created
     *  @return Single Cam instance
     **/
    static Cam &get_instance(int index = 0);

    /**
     * TODO: add distortion fix
     * @brief read frame and fix distortion based on intrinsic matrix
     * @param image: cv::Mat to be written
     * @return reference of self
     **/
    Cam &operator>>(cv::Mat &image);

    /**
     * @brief get the horizontal and vertical angle of the pixel in the frame
     * @param pixel: pixel in cv::Point
     * @return angles of the pixel in order (horizontal, vertical)
     **/
    cv::Point2d get_angles(const cv::Point2f &pixel);

    /**
     * @brief change the origin of the image to center
     * @return the position in new coordinate
     **/
    cv::Point2f center_origin(const cv::Point2f &pixel);

    /**
     * @brief change the origin of the image to left-up corner
     * @return the position in new coordinate
     **/
    cv::Point2f corner_origin(const cv::Point2f &pixel);

  private:
    cv::VideoCapture cap;
    static Cam *self;
    const cv::Point2f shift_val =
        cv::Point2f((float)pwidth / 2, (float)pheight / 2);

    /**
     * @brief default constructor
     * @param index: index to open cv::VideoCapture
     * @return Cam instance
     **/
    Cam(int index = 0);
};

/** @brief test the functionality of the camera */
void test_Cam();

#endif // CAMERA_H_
