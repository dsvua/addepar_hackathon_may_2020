
#include "types.h"
#include "realsense_camera.h"
#include "image_pipeline.h"
#include <iostream>
#include <signal.h>

using namespace std;

static bool quit = false;

static void signal_handle(int signum)
{
    printf("Quit due to exit command from user!\n");
    quit = true;
}


int main(int argc, char * argv[]) {
    Jetracer::context_t ctx;

    /* Register a shuwdown handler to ensure
       a clean shutdown if user types <ctrl+c> */
    struct sigaction sig_action;
    sig_action.sa_handler = signal_handle;
    sigemptyset(&sig_action.sa_mask);
    sig_action.sa_flags = 0;
    sigaction(SIGINT, &sig_action, NULL);


    // start camera capturing
    Jetracer::realsenseD435iThread jetracer_depth_camera(&ctx);
    jetracer_depth_camera.initialize();
    jetracer_depth_camera.waitRunning(ctx.wait_for_thread);

    // start commucation thread
    Jetracer::communicationThread jetracer_communication_thread(&ctx);
    jetracer_communication_thread.initialize();
    jetracer_communication_thread.waitRunning(ctx.wait_for_thread);


    // start image processing pipeline
    // Jetracer::imagePipelineThread jetracer_image_pipeline(&ctx);
    // jetracer_image_pipeline.initialize();
    // jetracer_image_pipeline.waitRunning(ctx.wait_for_thread);

    while(!quit){
        sleep(100);
    }

    // caught CTRL+C
    jetracer_communication_thread.threadShutdown();
    jetracer_depth_camera.threadShutdown();
}

// image ignore area: 60px on left and 50px on bottom. 0,0 is top left corner

// plane formula: ax + by + c = z
// fit plane to points: 
// Python example
// # do fit
// tmp_A = []
// tmp_b = []
// for i in range(len(xs)):
//     tmp_A.append([xs[i], ys[i], 1])
//     tmp_b.append(zs[i])
// b = np.matrix(tmp_b).T
// A = np.matrix(tmp_A)
// fit = (A.T * A).I * A.T * b
