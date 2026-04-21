#pragma once
#include <Servo.h>

#define SERVO_NUM 2
#define SERVO_DEFAULT_1 1500
#define SERVO_DEFAULT_2 1500
#define SERVO_PIN_1 9
#define SERVO_PIN_2 10
#define CATCH_DEG 1000

class Servo_lib
{
    public :
        Servo_lib()
        {
            
        }

        ~Servo_lib()
        {
            set_servo_default();
        }

        void Servo_init()
        {
            gw_servo[0].attach(SERVO_PIN_1, servo_min, servo_max);
            gw_servo[1].attach(SERVO_PIN_2, servo_min, servo_max);
            set_servo_default();
        }
        void set_PulseWidth2(int pulse1, int pulse2);
        int get_Pulse_val(int id);
        void set_PulseWidth_id(int id, int pulse);



        void set_servo_default()
        {
            pulse[0] = SERVO_DEFAULT_1;
            pulse[1] = SERVO_DEFAULT_2;
        }

    private :
        Servo gw_servo[SERVO_NUM];
        int pulse[SERVO_NUM];
        const int catch_deg = 1000;
        const int servo_min = 600;
        const int servo_max = 2400;

        void Reg_servo_lim(int* deg);
};