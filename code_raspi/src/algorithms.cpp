#include "algorithms.hpp"
#include "camera.hpp"
#include "classes.hpp"
#include "settings.hpp"

vector<Ball2D> detect_pp(const cv::Mat &frame)
{
    cv::Mat frame_hsv;
    // turns into HSV color space
    int kernel_size = 2 * Settings::MORPH_SIZE + 1;
    cv::GaussianBlur(frame, frame_hsv, cv::Size(kernel_size, kernel_size), 0);
    cv::cvtColor(frame_hsv, frame_hsv, cv::COLOR_BGR2HSV);

    // mask for ppball, plus dilations and erosions for smooth
    cv::Mat mask;
    cv::inRange(frame_hsv, Settings::COLOR_LOWER, Settings::COLOR_UPPER, mask);
    cv::Mat element = getStructuringElement(cv::MORPH_ELLIPSE,
                                            cv::Size(kernel_size, kernel_size));
    cv::erode(mask, mask, element);
    cv::dilate(mask, mask, element);
    // find contours in the image
    vector<vector<cv::Point>> contours;
    // only get most outside contour,
    // and compresses horizontal, vertical, and diagonal segments and
    // leaves only their end points. For example, an up-right rectangular
    // contour is encoded with 4 points.
    cv::findContours(mask, contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);

    // process the contours found
    vector<Ball2D> balls;
    balls.reserve(contours.size());
    for (auto &contour : contours)
    {
        double area = cv::contourArea(contour);
        // get the enclosing circle of the contour
        cv::Point2f center;
        float ball_radius;
        cv::minEnclosingCircle(contour, center, ball_radius);
        // check if the contour meets area requirement
        const double min_area =
            Settings::MIN_VISIBLE_RATIO * M_PI * ball_radius * ball_radius;
        if ((area < min_area) || ball_radius < Settings::MIN_BALL_R)
            continue;
        balls.push_back(Ball2D(center, ball_radius));
    }
    return balls;
}

void test_detection()
{
    Cam cam = Cam::get_instance(Settings::CAM_ID);
    cv::Mat frame;
    while (true)
    {
        cam >> frame;
        vector<Ball2D> balls = detect_pp(frame);
        vector<Ball3D> balls_3D;
        for (auto &ball : balls)
            balls_3D.push_back(ball.to_3D());
        std::cout << balls_3D.size() <<std::endl;
        for (auto &ball : balls_3D)
            ball.draw(frame);
        cv::imshow("result", frame);
        if (cv::waitKey(1) == 'q')
            break;
    }
}

void test_eigen()
{
    Eigen::MatrixXd m(3, 3);
    m << 1, 2, 3, 4, 5, 6, 7, 8, 9;
    std::cout << m << std::endl;
}
