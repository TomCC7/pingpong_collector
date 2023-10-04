#include "algorithms.hpp"
#include "camera.hpp"
#include "classes.hpp"
#include "controller.hpp"
#include "detector.hpp"
#include <mutex>
#include <thread>
#include <unistd.h>

/**
 * @brief test function to show that the ppball detection works
 **/
void test()
{
    // test_eigen();
    test_detection();
    // test_Cam();
    // test_to_3D();
    // test_nucleo();
}

/**
 * @brief main function
 **/
int main(int argc, char *argv[])
{
    // test();
    // return 0;
    std::thread detect_th(Detector::run);
    std::thread control_th(Controller::run);
    control_th.join();
    detect_th.join();
    return 0;
}
