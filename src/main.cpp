#include <Arduino.h>
#include "ADC_lib.hpp"
#include "Servo_lib.hpp"
#include "Task_Maneger.hpp"

#define SERIAL_BAUD 115200

//ここから
#include <stdarg.h>

void serial_printf(const char *format, ...) {
    char buf[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    Serial.print(buf);
}
//ここまでTeleplot用ライブラリ, 関数

Servo_lib servo;
ADC_lib adc;
Task_Maneger::task_flow flow = Task_Maneger::task_flow::STOP;
Task_Maneger::hand_state hand = Task_Maneger::hand_state::OPEN;
bool is_calibed = false;
bool is_ready = false;

volatile bool is_EMG_close = false;
volatile bool is_EMG_open = false;
volatile bool is_FSR1 = false;
volatile bool is_FSR2 = false;

void setup() {
  Serial.begin(SERIAL_BAUD);
  adc.ADC_init();
  servo.Servo_init();
} 

void loop() {
  //センサ値計測
  adc.set_ADC_val(analogRead(EMG_PIN_1), analogRead(EMG_PIN_2), analogRead(FSR_PIN_1), analogRead(FSR_PIN_2));

  switch(flow)   //動作フロー管理
  {
    case Task_Maneger::task_flow::STOP ://停止
      if(!is_calibed){
        flow = Task_Maneger::task_flow::CALIB;
      }
      else if(is_ready){
        flow = Task_Maneger::task_flow::WORK;
      }
      break;
    case Task_Maneger::task_flow::CALIB ://キャリブレーション
      if(is_calibed)
        flow = Task_Maneger::task_flow::WORK;
      break;

    case Task_Maneger::task_flow::WORK ://動作
      if(!is_ready){
        flow = Task_Maneger::task_flow::STOP;
      }
      else if(!is_calibed){
        flow = Task_Maneger::task_flow::CALIB;
      }
      break;
    
  }

  if(flow == Task_Maneger::task_flow::CALIB)
  {
    adc.EMG_Calibration();
    is_calibed = true;
  }

  if(flow == Task_Maneger::task_flow::WORK) //動作時のハンドのサーボ制御
  {
    switch(hand)  //ハンド状態
    {
      case Task_Maneger::hand_state::OPEN : //ハンド開く
        servo.set_servo_default();

        if(is_EMG_close){
          hand = Task_Maneger::hand_state::PRE_CATCH;
        }
        break;
      case Task_Maneger::hand_state::PRE_CATCH :  //把持モードで開いた状態
        servo.set_servo_default();

        if(is_FSR1 || is_FSR2){
          hand = Task_Maneger::hand_state::CATCH;
        }
        if(is_EMG_open)
          hand = Task_Maneger::hand_state::OPEN;
        break;
      case Task_Maneger::hand_state::CATCH :  //把持モードで閉じた状態
        if(is_FSR1)
          servo.set_PulseWidth_id(1, CATCH_DEG_1);
        if(is_FSR2)
          servo.set_PulseWidth_id(2, CATCH_DEG_2);

        if(is_EMG_open)
          hand = Task_Maneger::hand_state::OPEN;
        break;
    }
  }else{
    servo.set_servo_default();
  }

  /*
  if (adc.get_ADC_val(EMG_PIN_1) > 100) {
      servo.set_PulseWidth_id(1, CATCH_DEG);
  } else if (adc.get_ADC_val(EMG_PIN_1) < 50) {
      servo.set_servo_default();
  }  
  */

  //servo駆動関数
  servo.set_PulseWidth2(servo.get_Pulse_val(1), servo.get_Pulse_val(2));//ここを後で変更
 
  //シリアルモニター用
  char s[80];
  sprintf(s, "FSR = ch1 %d,  servo %d", adc.get_ADC_val(1), servo.get_Pulse_val(1));
  //sprintf(s,"ADC =ch1 %d, ch2  %d, ch3  %d, ch4  %d", adc.get_ADC_val(1), adc.get_ADC_val(2), adc.get_ADC_val(3), adc.get_ADC_val(4));
  Serial.println(s);
  //Teleplot用
  serial_printf(">FSR:%d\n", adc.get_ADC_val(EMG_PIN_1));
  serial_printf(">servo:%d", servo.get_Pulse_val(1));

  delay(100);//10Hz
}

