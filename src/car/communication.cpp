
#include "communication.h"
using namespace std;

namespace Jetracer {
    bool communicationThread::threadInitialize() {

        // Initialize the PWM board
        pca9685 = new PCA9685() ;
        int err = pca9685->openPCA9685();
        if (err < 0){
            printf("Error: %d", pca9685->error);
        }
        printf("PCA9685 Device Address: 0x%02X\n",pca9685->kI2CAddress);
        pca9685->setAllPWM(0,0);
        pca9685->reset();
        pca9685->setPWMFrequency(50);
        // Set the PWM to "neutral" (1.5ms)
        sleep(1);
        pca9685->setPWM(ESC_CHANNEL,0,PWM_NEUTRAL);
        _speed = 0;
        // pca9685->setPWM(ESC_CHANNEL,0,_speed);

        // start listening on given network port
        struct sockaddr_in addr;
        _socket = socket(AF_INET, SOCK_STREAM, 0);
         
        if (_socket < 0){
            printf("socket error №%i\n",errno);
            return false;
        }
         
        printf("sock= %i \n", _socket);
        memset(&addr, 0, sizeof(addr));
        addr.sin_family=AF_INET;
        addr.sin_port=htons(_ctx->listen_port);
        addr.sin_addr.s_addr= htonl(INADDR_ANY);
        const int trueFlag = 1;
        if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
            printf("Failure");
        if (bind(_socket,(struct sockaddr *)&addr,sizeof(addr)) == -1){
            printf("connect error  %i\n",errno);
            return false;
        }
         
        if(listen(_socket, 2) == -1){
            printf("listen() error!");
            return false;
        }

        _portIsOpen = true;
        return true;
    }

    bool communicationThread::threadExecute() {
        while (!m_doShutdown){
            printf("Creating connection\n");
            getIncomingConnection();

            printf("Starting video\n");
            // printf("/usr/bin/gst-launch-1.0 rpicamsrc bitrate=1000000" 
            //         " rotation=180 ! video/x-h264,width=640,height=480 "
            //         "! h264parse ! queue ! rtph264pay config-interval=1"
            //         " pt=96 ! udpsink host=192.168.192.13 port=9000\n");
            startVideo();

            while(!m_doShutdown && readMessage()){
                printf("Getting message\n");
                string message = getMessage();
                printf("Parsing message\n");
                parseMessage(message);
                printf("Message:%s\n", message.c_str());
               
            }
            stopVideo();
        }
        cout << "Stopping video" << endl;
        stopVideo();
        cout << "Video is stopped in communicationThread" << endl;
        return true;
    }

    bool communicationThread::portIsOpen(){
        return _portIsOpen;
    }


    bool communicationThread::sendBuffer(const char* buff, const int size){
        int sentSize = 0;
        while (sentSize < size){
            int len = write(_sd, buff + sentSize, size - sentSize);
            if (len < 0){
                printf("%s", strerror(errno));
                return false;
            }
            sentSize += len;
        }
        return true;
    }

    bool communicationThread::receiveBuffer(char* buff, const int size){
        int receivedSize = 0;
        while (receivedSize < size){
            int len = read(_sd, buff + receivedSize, size - receivedSize);
            if (len < 0){
                printf("receiveBuffer error %s", strerror(errno));
                return false;
            }
            if (len == 0){
                printf("Empty packet");
                break;
            }
            // printf("Received receiveBuffer size %i\n", len);
            receivedSize += len;
        }
        // printf("Size %i, receivedSize %i\n", size, receivedSize);
        return size == receivedSize;
    }


    bool communicationThread::sendMessage(const char* buff, const int size){
        char messageSizeStr[5] = {0};
        sprintf(messageSizeStr, "%d", size);
        printf("%d", size);
        sendBuffer(messageSizeStr, 2);
        return sendBuffer(buff, size);

    }

    bool communicationThread::readMessage(){
        char sizeBuff[5] = {0};
        if (receiveBuffer(sizeBuff, 2)){
            printf("Received readMessage %s\n", sizeBuff);
        } else return false;
        int size = atoi(sizeBuff);
        char* buff = new char[size];
        _buff = "";
        
        if (receiveBuffer(buff, size)){
            _buff = string(buff, size);
            delete[] buff;
            return true;
        }
        delete[] buff;
        return false;
    }

    std::string communicationThread::getMessage(){
        return _buff;
    }

    bool communicationThread::getIncomingConnection(){
        _sd = accept(_socket,NULL,NULL);
        
        //----------------------
        struct sockaddr     address;
        socklen_t           addresslen;
        struct sockaddr_in* addrInternet;
        std::string         ip;
        int port;
        char caddr[INET_ADDRSTRLEN];

        addresslen = sizeof(address);
        getpeername(_sd, &address, &addresslen);
        addrInternet = (struct sockaddr_in*)&address;
        
        port = ntohs(addrInternet->sin_port);
        inet_ntop(AF_INET, &(addrInternet->sin_addr), caddr, INET_ADDRSTRLEN);
        ip = inet_ntoa(addrInternet->sin_addr);
        
        //std::copy(std::begin(ipstr), std::end(ipstr), std::begin(client_ipstr_));
        printf("Peer IP address ntoa: %s\n", ip.c_str());
        printf("Peer IP address ntop: %s\n", caddr);
        printf("Peer port           : %d\n", port);
        //----------------------
       
        
        if (_sd == -1) {
            printf("error accept %i\n",errno);
            return false;
        }
        printf("Incoming socket %i\n", _sd);

        _ctx->client_ip_address = ip;
        // _ctx->client_port = port;
        return true;

    }

    void communicationThread::parseMessage(std::string message){
        std::vector<std::string> splitted_message = splitMessage(message, ':');
        int value = atoi(splitted_message[1].c_str());
        //value = atoi(strtok(cmessage, ":"));
        //value = atoi(cmessage);
        string command = splitted_message[0];
        cout << message << "|" << splitted_message[0] << " " << value << "|" << splitted_message[1] << endl;
        if (command == "Reset"){
            pca9685->reset();
        } else if (command == "Forward"){
            value = 1;
            _speed += value;
            if (_speed > 100){
                _speed = 100;
            }
            setPwmSpeed();
        } else if (command == "Backward"){
            value = 1;
            _speed -= value;
            if (_speed < -100) {
                 _speed = -100;
            }
            setPwmSpeed();
        } else if (command == "Right"){
            _steering = value;
            if (_steering > 100) {
                _steering = 100;
            }
            setPwmSteering();
        } else if (command == "Left"){
            _steering = 0 - value;
            if (_steering < -100) {
                _steering = -100;
            }
            setPwmSteering();
        }
        //delete cmessage;

    }

    void communicationThread::setPwmSpeed(){
        // if (_speed > -30 && _speed < 30) {
        //     std::cout << "Speed: " << _speed << std::endl;
        //     pca9685->setPWM(ESC_CHANNEL,0,PWM_NEUTRAL);
        // } else {
            int pwmSpeed = 0;
            if (_speed > 0 ){
                pwmSpeed = PWM_NEUTRAL + (PWM_FULL_FORWARD - PWM_NEUTRAL) * _speed / 100;
            } else {
                pwmSpeed = PWM_NEUTRAL + (PWM_NEUTRAL - PWM_FULL_REVERSE) * _speed / 100;
            }
            std::cout << "setPwmSpeed: " << pwmSpeed << std::endl;
            pca9685->setPWM(ESC_CHANNEL,0,pwmSpeed);
        // }        
    }

    void communicationThread::setPwmSteering(){
        // if (_speed > -30 || _speed < 30) {
        //     pca9685->setPWM(ESC_CHANNEL,0,PWM_NEUTRAL);
        // } else {
            int pwmSteering = 0;
            if (_steering > 0 ){
                pwmSteering = SERVO_PWM_NEUTRAL + SERVO_RIGHT_RANGE * _steering / 100;
                // pwmSteering = 4096/2 + (4096/2) * _steering / 100;
            } else {
                pwmSteering = SERVO_PWM_NEUTRAL + SERVO_LEFT_RANGE * _steering / 100;
                // pwmSteering = 4096/2 + (4096/2) * _steering / 100;
            }
            std::cout << "setPwmSteering: " << pwmSteering << std::endl;
            pca9685->setPWM(STEERING_CHANNEL,0,pwmSteering);
        // }        
    }

    void communicationThread::split(const string &s, char delim, vector<string> &elems){
        stringstream ss;
        ss.str(s);
        string item;
        while (getline(ss, item, delim)) {
            elems.push_back(item);
        }
    }

    vector<string> communicationThread::splitMessage(const string &s, char delim){
        vector<string> elems;
        split(s, delim, elems);
        return elems;
    }

    void communicationThread::startVideo(){
        _ctx->stream_video->set(true);
        // std::cout << "in communication _ctx->stream_video->get(): " << _ctx->stream_video->get() << std::endl;
        jetracer_video_stream = new videoStreamThread(_ctx);
        jetracer_video_stream->initialize();
        jetracer_video_stream->waitRunning();
    }

    void communicationThread::stopVideo(){
        jetracer_video_stream->shutdown();
        _ctx->stream_video->set(false);
    }

    bool communicationThread::threadShutdown(){
        return true;
    }


}