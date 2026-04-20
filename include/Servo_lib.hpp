#pragma once
#include <Servo.h>

#define SERVO_NUM 2
#define SERVO_DEFAULT_1 90
#define SERVO_DEFAULT_2 90
#define SERVO_PIN_1 9
#define SERVO_PIN_2 10

class Servo_lib
{
    public :
        Servo_lib()
        {
            gw_servo[0].attach(SERVO_PIN_1);
            gw_servo[1].attach(SERVO_PIN_2);
            set_servo_default();
        }

        ~Servo_lib()
        {
            set_servo_default();
        }
        void set_PulseWidth2(int pulse1, int pulse2);
        int get_Pulse_val(int id);



        void set_servo_default()
        {
            pulse[0] = SERVO_DEFAULT_1;
            pulse[1] = SERVO_DEFAULT_2;
            set_PulseWidth2(pulse[0], pulse[1]);
        }

    private :
        Servo gw_servo[SERVO_NUM];
        int pulse[SERVO_NUM];
        const int catch_arm_deg = 30;
        const int servo_min = 0;
        const int servo_max = 180;

        void Reg_servo_lim(int* deg);
};