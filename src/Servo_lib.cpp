#include "Servo_lib.hpp"

void Servo_lib::set_PulseWidth2(int pulse1, int pulse2)
{
    pulse[0] = pulse1;
    pulse[1] = pulse2;
    Reg_servo_lim(pulse);
    
    gw_servo[0].write(pulse[0]);
    gw_servo[1].write(pulse[1]);
}


int Servo_lib::get_Pulse_val(int id)
{
    return pulse[id -1];
}

void Servo_lib::Reg_servo_lim(int* deg)
{
    if(deg[0] > servo_max)
        deg[0] = servo_max;
    if(deg[0] < servo_min)
        deg[0] = servo_min;
    if(deg[1] > servo_max)
        deg[1] = servo_max;
    if(deg[1] < servo_min)
        deg[1] = servo_min;
}