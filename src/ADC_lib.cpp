#include "ADC_lib.hpp"

void ADC_lib::set_ADC_val(int emg_val1, int emg_val2, int fsr_val1, int fsr_val2)
{
    adc_val[0] = emg_val1;
    adc_val[1] = emg_val2;
    adc_val[2] = fsr_val1;
    adc_val[3] = fsr_val2;
}

int ADC_lib::get_ADC_val(int id)
{
    return adc_val[id-1];
}

void ADC_lib::set_emg_th(int th_ID1, int th_ID2)
{
    emg_th[0] = th_ID1;
    emg_th[1] = th_ID2;
}

void ADC_lib::ADC_init()
{
    adc_val[EMG_PIN_1] = adc_val[EMG_PIN_2] =  0;
    for(int size = 0; size < BUF_SIZE; size++)
    {
        for(int id=0; id < ADC_NUM; id++){
            adc_buf[id][size] = 0;
        }
    }
}

void ADC_lib::cal_ADC_avg(int id)
{
    long val = 0;
    for(int size = 0; size < BUF_SIZE; size++)
    {
        val += (long)adc_buf[id-1][size];
    }
    adc_val[id-1] = (short)(val / BUF_SIZE);
    
}