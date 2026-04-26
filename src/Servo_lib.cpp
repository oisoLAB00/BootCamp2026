#include "Servo_lib.hpp"

void Servo_lib::set_PulseWidth2(int pulse1, int pulse2)
{
    pulse[ID_SERVO1] = pulse1;
    pulse[ID_SERVO2] = pulse2;
    Reg_servo_lim(pulse);
    
    gw_servo[ID_SERVO1].writeMicroseconds(pulse[0]);
    gw_servo[ID_SERVO2].writeMicroseconds(pulse[1]);
}


int Servo_lib::get_Pulse_val(int id)
{
    return pulse[id];
}

void Servo_lib::Reg_servo_lim(int* deg)
{
    for(int id = 0; id < SERVO_NUM; id++)
    {
        if(deg[id] > servo_max)
            deg[id] = servo_max;
        if(deg[id] < servo_min)
            deg[id] = servo_min;
    }

}

void Servo_lib::set_PulseWidth_id(int id, int pulse_usec)
{
    pulse[id] = pulse_usec;
}

