#include "camera.hpp"
#include <iostream>

// static variable must be specified outside of the class...
Cam *Cam::self = nullptr;

Cam::Cam(int index) : cap{cv::VideoCapture(index)}
{
    if (Cam::self == nullptr)
    {
        Cam::self = this;
        cap.set(cv::CAP_PROP_FRAME_WIDTH, Cam::pwidth);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, Cam::pheight);
        cap.set(cv::CAP_PROP_BUFFERSIZE, 1);
    }
    else
    {
        throw "reinitialization of camera";
    }
}

Cam &Cam::get_instance(int index)
{
    if (Cam::self == nullptr)
        Cam::self = new Cam(index);
    return *Cam::self;
}

Cam &Cam::operator>>(cv::Mat &image)
{
    this->cap >> image;
    return *this;
}

cv::Point2d Cam::get_angles(const cv::Point2f &pixel)
{
    // shift points
    cv::Point2f pixel_centered = center_origin(pixel);
    // calculate angle based on focal length in pixel
    cv::Point2d angles{};
    angles.x = atan((double)pixel_centered.x / fl_pixel);
    angles.y = atan((double)pixel_centered.y / fl_pixel);
    return angles;
}

cv::Point2f Cam::center_origin(const cv::Point2f &pixel)
{
    return pixel - shift_val;
}

cv::Point2f Cam::corner_origin(const cv::Point2f &pixel)
{
    return pixel + shift_val;
}

void test_Cam()
{
    using std::cout, std::endl;
    Cam cam = Cam::get_instance(0);
    // get_angles test
    cout << "angles for left-up point: " << cam.get_angles(cv::Point2f{0, 0}) << endl;
    cout << "angles for center point: " << cam.get_angles(cv::Point2f{320, 240}) << endl;
    cout << "angles for right-down point: " << cam.get_angles(cv::Point2f{639, 479}) << endl;
}
