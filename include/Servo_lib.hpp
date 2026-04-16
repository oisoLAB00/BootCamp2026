#pragma once
#include <Servo.h>

#define SERVO_NUM 2
#define SERVO_DEFAULT_1 1500
#define SERVO_DEFAULT_2 1500

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
        void set_PulseWidth2(int pulse1, int pulse2);
        int get_Pulse_val();

        void set_servo_default()
        {
            pulse[0] = SERVO_DEFAULT_1;
            pulse[1] = SERVO_DEFAULT_2;
        }

    private :
        Servo servo_gws;
        int pulse[SERVO_NUM];
        const int catch_arm_deg = 2000;


};