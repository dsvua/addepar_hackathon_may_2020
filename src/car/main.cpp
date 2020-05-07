
#include "types.h"
#include "realsense_camera.h"
#include "image_pipeline.h"

int main(int argc, char * argv[]) {
    Jetracer::context_t ctx;

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
}

// image ignore area: 60px on left and 50px on bottom. 0,0 is top left corner


// fit plane to points: plane formula ax + by + c = z
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
