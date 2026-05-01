#pragma once
#include <ESP32Servo.h>

#define SERVO_NUM 2

#define SERVO_PIN_1 16
#define SERVO_PIN_2 17
#define SERVO_PIN_3 18

#define ID_SERVO1 0
#define ID_SERVO2 1
#define ID_SERVO3 2

//把持時の角度を変更したいとき, ここを変更する(600~2400の範囲)
#define SERVO_DEFAULT_1 2100 //servo1のデフォルトの角度
#define SERVO_DEFAULT_2 2100 //servo2のデフォルトの角度
#define CATCH_DEG_1 900 //servo1の把持時の角度
#define CATCH_DEG_2 900 //servo2の把持時の角度

class Servo_lib
{
    public :
        Servo_lib()
        {
            set_servo_default();
        }

        ~Servo_lib()
        {
            set_servo_default();
        }

        void Servo_init()
        {
            gw_servo[ID_SERVO1].attach(SERVO_PIN_1, servo_min, servo_max);
            gw_servo[ID_SERVO2].attach(SERVO_PIN_2, servo_min, servo_max);
            set_servo_default();
        }
        void set_PulseWidth2(int pulse1, int pulse2);
        int get_Pulse_val(int id);
        void set_PulseWidth_id(int id, int pulse);



        void set_servo_default()
        {
            pulse[ID_SERVO1] = SERVO_DEFAULT_1;
            pulse[ID_SERVO2] = SERVO_DEFAULT_2;
        }

    private :
        Servo gw_servo[SERVO_NUM];
        int pulse[SERVO_NUM];
        const int servo_min = 600;
        const int servo_max = 2400;

        void Reg_servo_lim(int* deg);
};