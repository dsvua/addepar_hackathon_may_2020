/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StreamRtpClient.h
 * Author: serhiy
 *
 * Created on November 26, 2016, 3:13 PM
 */

#ifndef STREAMRTPCLIENT_H
#define STREAMRTPCLIENT_H

#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <stdlib.h>
#include <QString>
#include <QDebug>


class StreamRtpClient {
public:
    StreamRtpClient();
    StreamRtpClient(const StreamRtpClient& orig);
    virtual ~StreamRtpClient();
private:
    QString init_line_;
    GstElement *pipeline;
    GError *error;
    GstElement *sink;
    GstBus *bus;
    
    static GstFlowReturn new_preroll(GstAppSink *appsink, gpointer data);
    static GstFlowReturn new_sample(GstAppSink *appsink, gpointer data);
    static gboolean my_bus_callback (GstBus *bus, GstMessage *message, gpointer data);
    
    bool initializeGst();
};

#endif /* STREAMRTPCLIENT_H */

