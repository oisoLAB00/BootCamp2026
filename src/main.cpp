#include <Arduino.h>
#include "ADC_lib.hpp"
#include "Servo_lib.hpp"
#include "Task_Maneger.hpp"

#define SERIAL_BAUD 115200

Servo_lib servo;
ADC_lib adc;
Task_Maneger task;
Task_Maneger::task_flow flow;
Task_Maneger::hand_state hand;

void setup() {
  Serial.begin(SERIAL_BAUD);
  flow = Task_Maneger::task_flow::CALIB;
} 

void loop() {
  task.set_task(flow, hand);
  adc.set_ADC_val(analogRead(EMG_PIN_1), analogRead(EMG_PIN_2), analogRead(FSR_PIN_1), analogRead(FSR_PIN_2));
  servo.set_PulseWidth2(servo.get_Pulse_val(1), servo.get_Pulse_val(2));

}

