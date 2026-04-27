#include "WiFi_GUI.hpp"


WiFi_GUI::WiFi_GUI() : server(80) {}


void WiFi_GUI::begin() {
    WiFi.softAP("SENSOR_GUI", "12345678");

    Serial.println(WiFi.softAPIP());

    if(!LittleFS.begin()){
        Serial.println("LittleFS mount failed");
        return;
    }

    setupRoutes();

    server.begin();
}

void WiFi_GUI::setupRoutes() {

    server.on("/", HTTP_GET, [this]() {
        handle_RootPage();
    });

    server.on("/start_calib", HTTP_GET, [this]() {
        handle_StartCalib();
    });

    server.on("/stop", HTTP_GET, [this]() {
        handle_StopMode();
    });

    server.on("/work", HTTP_GET, [this]() {
        handle_WorkMode();
    });

    server.on("/sensor", HTTP_GET, [this]() {
        handle_SensorData();
    });

    server.on("/app.js", HTTP_GET, [this]() {
        File file = LittleFS.open("/app.js", "r");

        if(!file){
            server.send(404, "text/plain", "app.js not found");
            return;
        }

        server.streamFile(file, "application/javascript");
        file.close();
    });
}


void WiFi_GUI::handle_StartCalib() {
    //uiCommand.start_calib = true;
    server.send(200, "text/plain", "calibration started");
}

void WiFi_GUI::handle_StopMode() {
    //uiCommand.force_stop = true;
    server.send(200, "text/plain", "stop mode");
}

void WiFi_GUI::handle_WorkMode() {
    //uiCommand.force_work = true;
    server.send(200, "text/plain", "work mode");
}

void WiFi_GUI::handle_SensorData() {
    String json = createSensorJson();
    server.send(200, "application/json", json);
}

void WiFi_GUI::handle_RootPage() {
    File file = LittleFS.open("/index.html", "r");

    if(!file){
        server.send(404, "text/plain", "index.html not found");
        return;
    }

    server.streamFile(file, "text/html");
    file.close();
}

String WiFi_GUI::createSensorJson() {
    String json = "{";

    json += "\"emg1\":" + String(send.emg1) + ",";
    json += "\"emg2\":" + String(send.emg2) + ",";
    json += "\"fsr1\":" + String(send.fsr1) + ",";
    json += "\"fsr2\":" + String(send.fsr2) + ",";
    json += "\"mode\":" + String(send.mode);

    json += "}";

    return json;
}


void WiFi_GUI::set_send_data(int emg_1, int emg_2, int fsr_1, int fsr_2, Task_Maneger::task_flow flow)
{
    send.emg1 = emg_1;
    send.emg2 = emg_2;
    send.fsr1 = fsr_1;
    send.fsr2 = fsr_2;
    switch(flow)
    {
        case Task_Maneger::task_flow::STOP :
            send.mode = 0;
            break;
        case Task_Maneger::task_flow::CALIB :
            send.mode = 1;
            break;
        case Task_Maneger::task_flow::WORK :
            send.mode = 2;
            break;
        default :
            send.mode = -1;
    }
}