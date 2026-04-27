#pragma once
#include <WiFi.h>
#include <WebServer.h>

class WiFi_GUI
{
    public :
        WiFi_GUI();

        void begin();
        void update(){
            server.handleClient();
        }

        //void sendSensorData(const SystemState& state);
    
    private :
        WebServer server;
        void setupRoutes();
        String createSensorJson();

        void handle_StartCalib();
        void handle_StopMode();
        void handle_WorkMode();
        void handle_SensorData();
        void handle_ThresholdUpdate();
        void handle_RootPage();

};