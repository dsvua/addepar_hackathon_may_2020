#include "video_stream.h"

namespace Jetracer {

    bool videoStreamThread::threadInitialize() {
        video_writer.open("appsrc ! video/x-raw,format==(string)GRAY8, width=(int)848, height=(int)480  ! nvvidconv ! 'video/x-raw(memory:NVMM), format=(string)I420’ ! omxh264enc ! 'video/x-h264, stream-format=byte-stream ! rtph264pay mtu=1400 ! udpsink host=$CLIENT_IP port=5000 sync=false async=false", 
                CAP_GSTREAMER, 0, (double)60, cv::Size(_ctx->cam_w, _ctx->cam_h), true);
    }
    
    bool videoStreamThread::threadExecute() {
    }

    bool videoStreamThread::threadShutdown() {
        return true;
    }


} // end of namespace Jetracer
