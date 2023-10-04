#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

#include "classes.hpp"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <Eigen/Dense>
using std::vector;

/**
 * @brief detect pingpong balls from the frame given
 * @return a list of pingpong balls detected
 **/
vector<Ball2D> detect_pp(const cv::Mat &frame);

/** @brief test the detect_pp function */
void test_detection();

void test_eigen();

#endif // DETECT_H_
