#include "video_stream.h"

namespace Jetracer {

    bool videoStreamThread::threadInitialize() {
        std::string gst_pipeline = fmt::format(
            "appsrc ! video/x-raw,format==(string)GRAY8, width=(int){}, height=(int){}  ! nvvidconv ! 'video/x-raw(memory:NVMM), format=(string)I420’ ! omxh264enc ! 'video/x-h264, stream-format=byte-stream ! rtph264pay mtu=1400 ! udpsink host={} port=5000 sync=false async=false",
            _ctx->cam_w, _ctx->cam_h, _ctx->client_ip_address
            );
        video_writer.open(gst_pipeline, 0, (double)_ctx->fps, cv::Size(_ctx->cam_w, _ctx->cam_h), true);
    }
    
    bool videoStreamThread::threadExecute() {
    }

    bool videoStreamThread::threadShutdown() {
        return true;
    }


} // end of namespace Jetracer
