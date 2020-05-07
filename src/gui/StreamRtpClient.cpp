/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StreamRtpClient.cpp
 * Author: serhiy
 * 
 * Created on November 26, 2016, 3:13 PM
 */

#include "StreamRtpClient.h"

StreamRtpClient::StreamRtpClient() {
    init_line_ = "-v udpsrc port=9000 "
            "caps='application/x-rtp, media=(string)video, "
            "clock-rate=(int)90000, encoding-name=(string)H264' ! "
            "rtph264depay ! avdec_h264 ! videoconvert ! "
            "appsink name=sink sync=true";
    
}

bool StreamRtpClient::initializeGst(){
    error = NULL;
    gst_init(NULL, NULL);
    gchar *descr = g_strdup(init_line_.toStdString().c_str());
    pipeline = gst_parse_launch (descr, &error);
    
    if (error != NULL) {
        qDebug() << "could not construct pipeline: " << error->message;
        g_error_free (error);
        return false;
    }
    
    sink = gst_bin_get_by_name (GST_BIN (pipeline), "sink");
    gst_app_sink_set_emit_signals((GstAppSink*)sink, true);
    gst_app_sink_set_drop((GstAppSink*)sink, true);
    gst_app_sink_set_max_buffers((GstAppSink*)sink, 1);
    GstAppSinkCallbacks callbacks = { NULL, new_preroll, new_sample };
    gst_app_sink_set_callbacks (GST_APP_SINK(sink), &callbacks, NULL, NULL);
    
    guint bus_watch_id;
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    bus_watch_id = gst_bus_add_watch (bus, my_bus_callback, NULL);
    gst_object_unref (bus);

    gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_PLAYING);
    return true;
}

StreamRtpClient::StreamRtpClient(const StreamRtpClient& orig) {
}

StreamRtpClient::~StreamRtpClient() {
}

gboolean StreamRtpClient::my_bus_callback (GstBus *bus, GstMessage *message, gpointer data) {
  g_print ("Got %s message\n", GST_MESSAGE_TYPE_NAME (message));
  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ERROR: {
      GError *err;
      gchar *debug;

      gst_message_parse_error (message, &err, &debug);
      g_print ("Error: %s\n", err->message);
      g_error_free (err);
      g_free (debug);    
      break;
    }
    case GST_MESSAGE_EOS:
      /* end-of-stream */
      break;
    default:
      /* unhandled message */
      break;
  }
}
  
GstFlowReturn StreamRtpClient::new_sample(GstAppSink *appsink, gpointer data) {
  static int framecount = 0;
  framecount++;

  GstSample *sample = gst_app_sink_pull_sample(appsink);
  GstCaps *caps = gst_sample_get_caps(sample);
  GstBuffer *buffer = gst_sample_get_buffer(sample);
  const GstStructure *info = gst_sample_get_info(sample);

  GstMapInfo map;
  gst_buffer_map(buffer, &map, GST_MAP_READ);

  gst_buffer_unmap(buffer, &map);

  // ------------------------------------------------------------

  // print dot every 30 frames
  if (framecount%30 == 0) {
    g_print (".");
  }

  // show caps on first frame
  if (framecount == 1) {
    g_print ("%s\n", gst_caps_to_string(caps));
  }

  gst_sample_unref (sample);
  return GST_FLOW_OK;
}

GstFlowReturn StreamRtpClient::new_preroll(GstAppSink *appsink, gpointer data) {
  g_print ("Got preroll!\n");
  return GST_FLOW_OK;
}