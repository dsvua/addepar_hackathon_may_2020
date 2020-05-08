#include "video_stream.h"

using namespace cv;
using namespace rs2;

namespace Jetracer {

    bool videoStreamThread::threadInitialize() {
        // std::string gst_pipeline = fmt::format(
        //     "appsrc ! video/x-raw,format=(string)GRAY8,width=(int){},height=(int){} ! nvvidconv ! video/x-raw(memory:NVMM),format=(string)I420 ! omxh264enc iframeinterval=100  control-rate=2 bitrate=1000000 ! video/x-h264,stream-format=byte-stream ! rtph264pay mtu=1400 ! udpsink host={} port={} sync=false async=false",
        //     _ctx->cam_w, _ctx->cam_h, _ctx->client_ip_address, _ctx->client_port
        //     );
        std::string gst_pipeline = fmt::format(
            "appsrc ! video/x-raw,format=(string)GRAY8,width=(int){},height=(int){} ! nvvidconv ! video/x-raw(memory:NVMM),format=(string)I420 ! omxh264enc preset-level=0 ! video/x-h264,stream-format=byte-stream ! rtph264pay mtu=1400 ! udpsink host={} port={} sync=false async=false",
            _ctx->cam_w, _ctx->cam_h, _ctx->client_ip_address, _ctx->client_port
            );
        std::cout << "gst_pipeline: " << gst_pipeline << std::endl;
        bool is_color = false;
        int codec = cv::VideoWriter::fourcc('4', '0', ' ', ' '); // grayscale as in https://softron.zendesk.com/hc/en-us/articles/207695697-List-of-FourCC-codes-for-video-codecs
        video_writer.open(gst_pipeline, CAP_GSTREAMER, 0, (double)_ctx->fps, cv::Size(_ctx->cam_w, _ctx->cam_h), is_color);
        // check if we succeeded
        if (!video_writer.isOpened()) {
            std::cerr << "Could not open the output video file for write\n";
            return false;
        }
        return true;
    }
    
    bool videoStreamThread::threadExecute() {
        while (!m_doShutdown){
            video_frame tframe = frame{};
            if (_ctx->left_ir_queue->poll_for_frame(&tframe))
            {
                // std::cout << "Frame count: " << tframe.get_frame_metadata(RS2_FRAME_METADATA_FRAME_COUNTER) << std::endl;
                int image_size;
                Mat image = Mat(cv::Size(_ctx->cam_w, _ctx->cam_h), CV_8UC1, (void*)tframe.get_data());
                video_writer << image;
            }
        }
        std::cout << "Stopping videoStreamThread" << std::endl;
        // video_writer.close();
        return true;
    }

    bool videoStreamThread::threadShutdown() {
        return true;
    }


} // end of namespace Jetracer
