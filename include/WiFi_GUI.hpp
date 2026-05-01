#pragma once
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include "Task_Maneger.hpp"

#define GUI_SSID "SENSOR_GUI"
#define GUI_PASS "12345678"

struct send_data
{
    int emg1;
    int emg2;
    int fsr1;
    int fsr2;
    int mode;
};


class WiFi_GUI
{
    public :
        WiFi_GUI();

        void begin();
        void update(){
            server.handleClient();
        }

        void set_send_data(int emg_1, int emg_2, int fsr_1, int fsr_2, Task_Maneger::task_flow flow);

        //void sendSensorData(const SystemState& state);
    
    private :
        WebServer server;
        send_data send;
        void Reset_sendData();
        void setupRoutes();
        String createSensorJson();

        void handle_StartCalib();
        void handle_StopMode();
        void handle_WorkMode();
        void handle_SensorData();
        void handle_ThresholdUpdate();
        void handle_RootPage();
        

};