#include <Arduino.h>
#include "ADC_lib.hpp"
#include "Servo_lib.hpp"
#include "Task_Maneger.hpp"

#define SERIAL_BAUD 115200
#include <stdarg.h>

void serial_printf(const char *format, ...) {
    char buf[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    Serial.print(buf);
}

Servo_lib servo;
ADC_lib adc;
Task_Maneger::task_flow flow = Task_Maneger::task_flow::STOP;
Task_Maneger::hand_state hand = Task_Maneger::hand_state::OPEN;

void setup() {
  Serial.begin(SERIAL_BAUD);
} 

void loop() {
  switch(flow)
  {
    case Task_Maneger::task_flow::CALIB :
      break;

    case Task_Maneger::task_flow::WORK :
      break;

    case Task_Maneger::task_flow::STOP :
      break;
    
  }

  if(flow == Task_Maneger::task_flow::WORK)
  {
    switch(hand)
    {
      case Task_Maneger::hand_state::OPEN :
        break;
      case Task_Maneger::hand_state::PRE_CATCH :
        break;
      case Task_Maneger::hand_state::CATCH :
        break;
    }
  }
  adc.set_ADC_val(analogRead(EMG_PIN_1), analogRead(EMG_PIN_2), analogRead(FSR_PIN_1), analogRead(FSR_PIN_2));
  servo.set_PulseWidth2(servo.get_Pulse_val(1), servo.get_Pulse_val(2));

  char s[80];

  sprintf(s, "FSR = ch1 %d", adc.get_ADC_val(1));
  //sprintf(s,"ADC =ch1 %d, ch2  %d, ch3  %d, ch4  %d", adc.get_ADC_val(1), adc.get_ADC_val(2), adc.get_ADC_val(3), adc.get_ADC_val(4));
  Serial.println(s);
  serial_printf(">FSR:%d\n", adc.get_ADC_val(1));

  delay(100);
}

