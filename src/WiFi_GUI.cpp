#include "WiFi_GUI.hpp"

WiFi_GUI::WiFi_GUI() : server(80) {}

void WiFi_GUI::begin() {
    Reset_sendData();
    
    WiFi.softAP("SENSOR_GUI", "12345678");

    Serial.print("AP IP Address: ");
    Serial.println(WiFi.softAPIP());

    //-----------------------------------
    // 開発中のみ true 推奨
    // mount失敗時に自動フォーマット
    //-----------------------------------
    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS mount failed");
        return;
    }

    Serial.println("LittleFS mount success");

    //-----------------------------------
    // 保存ファイル確認
    //-----------------------------------
    Serial.println("=== LittleFS Files ===");

    File root = LittleFS.open("/");
    File file = root.openNextFile();

    while (file) {
        Serial.print("File: ");
        Serial.println(file.name());
        file = root.openNextFile();
    }

    Serial.println("======================");

    setupRoutes();

    server.begin();
    Serial.println("WebServer started");
}

void WiFi_GUI::Reset_sendData()
{
    send.emg1 = 0;
    send.emg2 = 0;
    send.fsr1 = 0;
    send.fsr2 = 0;
    send.mode = 0;
}

void WiFi_GUI::setupRoutes() {

    //-------------------------
    // root page（これ追加）
    //-------------------------
    server.on("/", HTTP_GET, [this]() {
        File file = LittleFS.open("/index.html", "r");

        if (!file) {
            server.send(404, "text/plain", "index.html not found");
            return;
        }

        server.streamFile(file, "text/html");
        file.close();
    });

    //-------------------------
    // 静的ファイル（JSなど）
    //-------------------------
    server.on("/app.js", HTTP_GET, [this]() {
        File file = LittleFS.open("/app.js", "r");
        server.streamFile(file, "application/javascript");
        file.close();
    });

    server.on("/chart.js", HTTP_GET, [this]() {
        File file = LittleFS.open("/chart.js", "r");
        server.streamFile(file, "application/javascript");
        file.close();
    });

    //-------------------------
    // API
    //-------------------------
    server.on("/sensor", HTTP_GET, [this]() {
        handle_SensorData();
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
}

void WiFi_GUI::handle_StartCalib() {
    Serial.println("start calibration command");

    // uiCommand.start_calib = true;
    server.send(200, "text/plain", "calibration started");
}

void WiFi_GUI::handle_StopMode() {
    Serial.println("stop command");

    // uiCommand.force_stop = true;
    server.send(200, "text/plain", "stop mode");
}

void WiFi_GUI::handle_WorkMode() {
    Serial.println("work command");

    // uiCommand.force_work = true;
    server.send(200, "text/plain", "work mode");
}

void WiFi_GUI::handle_SensorData() {
    String json = createSensorJson();

    server.send(200, "application/json", json);
}

void WiFi_GUI::handle_RootPage() {
    File file = LittleFS.open("/index.html", "r");

    if (!file) {
        Serial.println("index.html not found");
        server.send(404, "text/plain", "index.html not found");
        return;
    }

    Serial.println("send index.html");

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

void WiFi_GUI::set_send_data(int emg_1, int emg_2, int fsr_1, int fsr_2, Task_Maneger::task_flow flow) {
    send.emg1 = emg_1;
    send.emg2 = emg_2;
    send.fsr1 = fsr_1;
    send.fsr2 = fsr_2;

    switch (flow) {
        case Task_Maneger::task_flow::STOP:
            send.mode = 0;
            break;

        case Task_Maneger::task_flow::CALIB:
            send.mode = 1;
            break;

        case Task_Maneger::task_flow::WORK:
            send.mode = 2;
            break;

        default:
            send.mode = -1;
            break;
    }
}