
#ifndef JETRACER_COMMUNICATION_THREAD_H
#define JETRACER_COMMUNICATION_THREAD_H

#include "Thread.h"
#include "constants.h"
#include "types.h"
#include "video_stream.h"
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <iostream>


namespace Jetracer {

    class communicationThread : public Thread {
    public:
        explicit communicationThread(context_t * ctx)
                                    : _ctx(ctx)
        {
        }
        ~communicationThread() {}
        // rs2::frame_queue depth_queue(CAPACITY);
        // rs2::frame_queue left_ir_queue(CAPACITY);
        // rs2::frame_queue right_ir_queue(CAPACITY);
        bool readMessage();
        bool sendMessage(const char* buff, const int size);
        bool portIsOpen();
        std::string getMessage();
        bool getIncomingConnection();
        void parseMessage(string message);
        void startVideo();
        void stopVideo();
    private:
        virtual bool threadInitialize();
        virtual bool threadExecute();
        virtual bool threadShutdown();
        

        int    _socket;
        int    _sd; //socket descriptor
        bool   _portIsOpen;
        int    _speed; // -100..0..100
        std::string _buff;
        bool sendBuffer(const char* buff, const int size);
        bool receiveBuffer(char* buff, const int size);
        void split(const string &s, char delim, vector<string> &elems);
        void setPwmSpeed();
        std::vector<string> splitMessage(const string &s, char delim);
        context_t * _ctx;
        std::shared_ptr<videoStreamThread> * jetracer_video_stream;
        PCA9685 *pca9685;

    } // end of class
} // end of namespace Jetracer

#endif // JETRACER_COMMUNICATION_THREAD_H
