#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include "detector.hpp"
#include "nucleo.hpp"

namespace Controller
{
using std::string;
enum ControlStates
{
    SEARCH,
    ROTATE,
    FETCH
};

/**
 * @brief run the controller
 **/
void run();

class PID
{
    private:
        double kp, ki, kd;
        // previous input
        double pinput = 0;
        bool init = false;
        // integral
        double integral = 0;
    public:
        /**
         * @brief PID constructor
         * @param kp: P parameter
         * @param ki: I parameter
         * @param kd: D parameter
         */
        PID(double kp, double ki, double kd);

        /**
         * @brief update PID status
         * @param input: new input value
         * @return calculated output
         **/
        double update(double input);
};
} // namespace Controller

#endif // CONTROLLER_H_
