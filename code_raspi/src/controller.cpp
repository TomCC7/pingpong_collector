#include "controller.hpp"
#include "settings.hpp"
#include <iostream>

namespace Controller
{
void run()
{
    // connect to device
    Nucleo nucleo(Settings::SERIAL_PORT);
    bool is_fetching = false;
    uint8_t balls_collected = 0;
    // uint8_t balls_detected = 0; TODO: add this
    ControlStates state = SEARCH;
    vector<Ball3D>::iterator it_ball_fetching;
    Ball3D prev_ball_fetching(cv::Point3d(), 0);
    // comparison
    auto fn_ball_cmp = [prev_ball_fetching](const Ball3D &a, const Ball3D &b) {
        return abs(a.distance - prev_ball_fetching.distance) <
               abs(b.distance - prev_ball_fetching.distance);
    };
    // wait for detection thread
    sleep(1);
    while (true)
    {
        // get balls
        std::vector<Ball3D> balls = Detector::get_result();
        // if no ball, keep rotating and searching
        // std::cout << "controller: " << balls.size() << std::endl;
        if (balls.empty())
        {
            nucleo.rotate(Settings::ROT_SPEED, false);
            // TODO: check the threshold
            if (state == FETCH)
            {
                // sleep for the ball to be reached
                nucleo.go_straight(Settings::SPEED);
                nucleo.set_servo(0);
                sleep(1);
                balls_collected++;
                nucleo.send_ball_numbers(0,balls_collected);
                std::cout << "ball catched!" << std::endl;
                it_ball_fetching = vector<Ball3D>::iterator();
                nucleo.go_straight(0);
                nucleo.set_servo(100);
            }
            state = SEARCH;
            continue;
        }

        // find nearest ball
        it_ball_fetching =
            std::min_element(balls.begin(), balls.end(), fn_ball_cmp);
        double dist_diff =
            abs(it_ball_fetching->distance - prev_ball_fetching.distance);
        switch (state)
        {
        case SEARCH:
            state = ROTATE;
            break;
        case ROTATE:
            if (abs(it_ball_fetching->center.x) > Settings::CENTER_THRESH)
            {
                // rotate to center the ball
                bool dir = (it_ball_fetching->center.x > 0);
                nucleo.rotate(Settings::ROT_SPEED, dir);
            }
            else
            {
                state = FETCH;
            }
            break;
        case FETCH:
            if (abs(it_ball_fetching->center.x) > Settings::CENTER_THRESH)
            {
                state = ROTATE;
                continue;
            }
            // go straight
            nucleo.go_straight(Settings::SPEED);
            // TODO: add thresh
            // if (dist_diff > 50 && prev_ball_fetching.center.y < ) sleep(1);
            // if (dist_diff > 50)
            //     sleep(2);
        }
        // update previous ball
        prev_ball_fetching = *it_ball_fetching;
    }
}

PID::PID(double _kp, double _ki, double _kd) : kp(_kp), ki(_ki), kd(_kd)
{
}

double PID::update(double input)
{
    if (!init)
    {
        init = true;
        pinput = input;
    }
    integral += input;
    return kp * input + ki * integral + kd * (input - pinput);
}
} // namespace Controller
