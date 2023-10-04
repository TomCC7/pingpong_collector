#ifndef DETECT_H_
#define DETECT_H_
#include "classes.hpp"
#include "algorithms.hpp"
#include <mutex>
#include <thread>
#include <vector>

#include <chrono>
using std::vector;
using namespace std::chrono;

class Detector
{
  public:
    /**
     * @brief run the detector
     **/
    static void run();
    static std::vector<Ball3D> get_result();
    // no instance
    Detector()=delete;

  private:
    static std::mutex mtx;
    static std::vector<Ball3D> buffer;
};

#endif // DETECT_H_
