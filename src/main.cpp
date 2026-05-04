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

bool is_shift = false;
bool is_shift_OK = true;
bool is_EMG_close = false;
bool is_EMG_open = false;
bool is_FSR1 = false;
bool is_FSR2 = false;

int shift_cnt = 0;

void setup() {
  Serial.begin(SERIAL_BAUD);
  adc.ADC_init();
  servo.Servo_init();
} 

void loop() {
  //センサ値計測
  adc.set_ADC_val(analogRead(EMG_PIN_1), analogRead(EMG_PIN_2), analogRead(FSR_PIN_1), analogRead(FSR_PIN_2));

  bool is_emg_active = is_EMG_open || is_EMG_close;

  is_shift = false;

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
    is_ready = true;
  }

  if(flow == Task_Maneger::task_flow::WORK) //動作時のハンドのサーボ制御
  {
    switch(hand)  //ハンド状態
    {
      case Task_Maneger::hand_state::OPEN : //ハンド開く
        servo.set_servo_default();

        if(is_FSR1){
          servo.set_PulseWidth_id(1, CATCH_DEG_1);
          if(is_emg_active){
            servo.set_PulseWidth_id(1, SERVO_DEFAULT_1);
            is_shift_OK = false;
            break;
          }
        }
        if(is_FSR2){
          servo.set_PulseWidth_id(2, CATCH_DEG_2);
          if(is_emg_active){
            servo.set_PulseWidth_id(2, SERVO_DEFAULT_2);
            is_shift_OK = false;
            break;
          }
        }

        if(is_emg_active){
          is_shift = true;
        }
        break;
      case Task_Maneger::hand_state::CATCH :  //把持モードで閉じた状態
        servo.set_PulseWidth_id(1, CATCH_DEG_1);
        servo.set_PulseWidth_id(2, CATCH_DEG_2);
        if(is_emg_active){
          is_shift = true;
        }
          
        break;
    }
  }else{
    servo.set_servo_default();
  }

  if(is_shift && is_shift_OK)//ハンド状態遷移
  {
    if(hand == Task_Maneger::hand_state::OPEN){
      hand = Task_Maneger::hand_state::CATCH;
    }
    else if(hand == Task_Maneger::hand_state::CATCH){
      hand = Task_Maneger::hand_state::OPEN;
    }
    is_shift = false;
    is_shift_OK = false;
    shift_cnt = 0;
  }

  if(!is_shift_OK)//ハンド状態が筋電によって遷移した場合に, ちょっと待つ
  {
    shift_cnt++;
    if(shift_cnt >= WAIT_TIME){
      is_shift_OK = true;
      shift_cnt = 0;
    }
  }

  //servo駆動関数
  servo.set_PulseWidth2(servo.get_Pulse_val(1), servo.get_Pulse_val(2));//ここを後で変更
 
  //シリアルモニター用
  char s[80];
  sprintf(s, "close %d,  open %d", (int)is_EMG_close, (int)is_EMG_open);
  //sprintf(s,"ADC =ch1 %d, ch2  %d, ch3  %d, ch4  %d", adc.get_ADC_val(1), adc.get_ADC_val(2), adc.get_ADC_val(3), adc.get_ADC_val(4));
  Serial.println(s);
  //Teleplot用
  serial_printf(">EMGclose:%d\n", adc.get_ADC_val(EMG_PIN_1));
  serial_printf(">EMGopen:%d\n", adc.get_ADC_val(EMG_PIN_2));
  serial_printf(">close_th:%d\n", adc.get_ADC_th(EMG_PIN_1));
  serial_printf(">open_th:%d\n", adc.get_ADC_th(EMG_PIN_2));
  //serial_printf(">FSR1:%d\n", adc.get_ADC_val(FSR_PIN_1));
  //serial_printf(">FSR2:%d\n", adc.get_ADC_val(FSR_PIN_2));
  serial_printf(">Servo1:%d\n", servo.get_Pulse_val(ID_SERVO_1));
  serial_printf(">Servo2:%d\n", servo.get_Pulse_val(ID_SERVO_2));

  delay(100);//10Hz
}

