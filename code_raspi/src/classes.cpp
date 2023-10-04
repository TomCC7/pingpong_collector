#include "classes.hpp"
#include "settings.hpp"

#include <iostream>
#include <opencv2/imgproc.hpp>

/**
 * Ball2D
 **/
Ball2D::Ball2D(cv::Point2f _center, double _radius)
    : center{_center}, radius{_radius}
{
}

void Ball2D::draw(cv::Mat &frame, cv::Scalar color, int thickness)
{
    cv::circle(frame, this->center, this->radius, color, thickness);
}

Ball3D Ball2D::to_3D()
{
    using std::sin;
    Cam cam = Cam::get_instance();
    // get angle of center
    cv::Point2d angles_center = cam.get_angles(this->center);
    cv::Point2f centered_center = cam.center_origin(this->center);
    // calculate distance
    // calculate the half-field angle of the ball
    cv::Point2f left_point =
        centered_center - cv::Point2f{(float)this->radius, 0};
    cv::Point2f right_point =
        centered_center + cv::Point2f{(float)this->radius, 0};
    double angle_left = cam.get_angles(left_point).x;
    double angle_right = cam.get_angles(right_point).x;
    double half_field_angle = abs(angle_left - angle_right) / 2;
    // calculate distance
    double dist = Settings::BALL_RADIUS / sin(half_field_angle);
    // calculate 3D position
    double a_x[3][3] = {{cos(angles_center.x), 0., -sin(angles_center.x)},
                        {0., 1., 0.},
                        {sin(angles_center.x), 0., cos(angles_center.x)}};
    cv::Mat rot_x = cv::Mat(3, 3, CV_64F, a_x).inv();
    double a_y[3][3] = {{1, 0, 0},
                        {0, cos(angles_center.y), -sin(angles_center.y)},
                        {0, sin(angles_center.y), cos(angles_center.y)}};
    cv::Mat rot_y = cv::Mat(3, 3, CV_64F, a_y).inv();
    cv::Mat dir = cv::Mat::zeros(cv::Size(1, 3), CV_64F);
    dir.at<double>(2) = 1;
    dir = rot_y * rot_x * dir;
    cv::Point3d pos = cv::Point3d(dir) * dist;
    return Ball3D(pos, dist, *this);
}

/**
 * Ball3D
 **/
Ball3D::Ball3D(cv::Point3d c, double d, const Ball2D &ball)
    : center{c}, distance{d}, ball_2d{ball}
{
}

Ball3D::Ball3D(cv::Point3d c, double d)
    : center{c}, distance{d}, ball_2d{Ball2D(cv::Point2d(), 0)}
{
}

void Ball3D::draw(cv::Mat &frame, cv::Scalar color, int thickness)
{
    cv::circle(frame, ball_2d.center, ball_2d.radius, color, thickness);
    std::string text = "Distance: " + std::to_string(this->distance);
    cv::putText(frame, text, cv::Point{ball_2d.center},
                cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar{0, 0, 0}, thickness);
}

/**
 * Bots
 **/
Bot::Bot() : loc(cv::Point3d(0, 0, 0))
{
}

void test_to_3D()
{
    using std::cout, std::endl;
    Ball2D ball2d(cv::Point2f(0, 0), 100);
    Ball3D ball3d = ball2d.to_3D();
    cout << "center: " << ball3d.center << endl;
    cout << "distance: " << ball3d.distance << endl;
}
