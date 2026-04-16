#include "ADC_lib.hpp"

void ADC_input::set_ADC_input(int* val)
{
    adc_val[0] = val[0];
    adc_val[1] = val[1];
}

int ADC_input::get_ADC_val(int id)
{
    return adc_val[id-1];
}

void ADC_input::set_emg_th(int th_ID1, int th_ID2)
{
    
}

