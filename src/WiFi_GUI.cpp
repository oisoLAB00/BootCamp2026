#include "WiFi_GUI.hpp"

void WiFi_GUI::begin() {
    WiFi.softAP("SENSOR_GUI", "12345678");

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
    server.send(200, "text/html", html_page);
}

String WiFi_GUI::createSensorJson() {
    String json = "{";

    json += "\"emg1\":" + String(systemState.sensor.emg1) + ",";
    json += "\"emg2\":" + String(systemState.sensor.emg2) + ",";
    json += "\"fsr1\":" + String(systemState.sensor.fsr1) + ",";
    json += "\"fsr2\":" + String(systemState.sensor.fsr2) + ",";
    json += "\"mode\":" + String((int)systemState.flow);

    json += "}";

    return json;
}