
#ifndef JETRACER_CONSTANTS_H
#define JETRACER_CONSTANTS_H

namespace Jetracer {

    #define CAPACITY 5 // allow max latency of 5 frames in a queue
    #define GPU_CAPACITY 3 // schedule 3 frames in parallel


    // The nomenclature is for the ESC throttle, for the steering neutral is center,
    // full forward is right, full reverse is left

    #define PWM_FULL_REVERSE 204 // 1ms/20ms * 4096
    #define PWM_NEUTRAL 307      // 1.5ms/20ms * 4096
    #define PWM_FULL_FORWARD 409 // 2ms/20ms * 4096

    // The Steering Servo is plugged into the follow PWM channel
    #define STEERING_CHANNEL 0
    // The ESC is plugged into the following PWM channel
    #define ESC_CHANNEL 1
}

#endif // JETRACER_CONSTANTS_H