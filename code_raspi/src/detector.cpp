#include "detector.hpp"
#include "settings.hpp"
#include <iostream>
#include <chrono>
using std::vector;
using namespace std::chrono;

std::vector<Ball3D> Detector::buffer{};
std::mutex Detector::mtx{};

void Detector::run()
{
    Cam cam = Cam::get_instance(Settings::CAM_ID);
    cv::Mat frame;
    while (true)
    {
        auto start = high_resolution_clock::now();
        cam >> frame;
        vector<Ball2D> balls = detect_pp(frame);
        vector<Ball3D> balls_3D;
        for (auto &ball : balls)
            balls_3D.push_back(ball.to_3D());

        // activate result
        Detector::mtx.lock();
        std::swap(Detector::buffer, balls_3D);
        Detector::mtx.unlock();

        // std::cout << "detector: " << balls_3D.size() << std::endl;

        auto time = duration_cast<microseconds>(high_resolution_clock::now() - start);
        double fps = 1.0e6 / time.count();
        std::cout << "fps: " << fps <<std::endl;
#ifdef DEBUG
        // draw functions
        for (auto &ball : balls_3D)
            ball.draw(frame);
        cv::imshow("result", frame);
        if (cv::waitKey(1) == 'q')
            break;
#endif
    }
    cv::destroyAllWindows();
}

std::vector<Ball3D> Detector::get_result()
{
    Detector::mtx.lock();
    auto ret = Detector::buffer;
    Detector::mtx.unlock();
    return ret;
}
