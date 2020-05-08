#ifndef JETRACER_TYPES_THREAD_H
#define JETRACER_TYPES_THREAD_H

#include "constants.h"
#include "Ordered.h"
#include <cuda.h>
#include <librealsense2/rs.hpp>

namespace Jetracer {

    typedef struct
    {
        unsigned int cam_w = 848;
        unsigned int cam_h = 480;
        unsigned int fps = 90;
        unsigned int frames_to_skip = 30; // discard all frames until start_frame to
                                       // give autoexposure, etc. a chance to settle
        unsigned int left_gap = 60; // ignore left 60 pixels on depth image as they
                                    // usually have 0 distance and useless
        unsigned int bottom_gap = 50; // ignore bottom 50 pixels on depth image

        unsigned int min_obstacle_height = 5; // ignore obstacles lower then 5mm
        unsigned int max_obstacle_height = 250; // ignore everything higher then 25cm
                                        // as car is not that tall

        rs2::frame_queue* depth_queue;
        rs2::frame_queue* left_ir_queue;
        rs2_intrinsics jetson_camera_intrinsics;

        Ordered<bool>* stream_video; // by default do not stream video
        std::string client_ip_address; // address of desktop/laptop that controls car
        int client_port = 9000; // port for video streaming/gstreamer that listens on control host
        int listen_port = 5000; // port to listen for commands
        useconds_t wait_for_thread = 1 * 1000000; // wait for 1 sec for thread to start

    } context_t;

} // namespace Jetracer

#endif // JETRACER_TYPES_THREAD_H
