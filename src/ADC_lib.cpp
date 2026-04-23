#include "ADC_lib.hpp"

void ADC_lib::set_ADC_val(int emg_val1, int emg_val2, int fsr_val1, int fsr_val2)
{

    adc_val[2] = fsr_val1;
    adc_val[3] = fsr_val2;

    if(fsr_val1 > FSR_TH_VAL){
        is_FSR1 = true;
    }else{
        is_FSR1 = false;
    }
    if(fsr_val2 > FSR_TH_VAL){
        is_FSR2 = true;
    }else{
        is_FSR2 = false;
    }

    for(int size = 0; size < (BUF_SIZE-1); size++)
    {
        adc_buf[0][size+1] = adc_buf[0][size];
    }
    adc_buf[0][0] = emg_val1;
    cal_ADC_avg(0);

    for(int size = 0; size < (BUF_SIZE-1); size++)
    {
        adc_buf[1][size+1] = adc_buf[1][size];
    }
    adc_buf[1][0] = emg_val2;
    cal_ADC_avg(1);

    adc_val[0] = emg_val1;
    adc_val[1] = emg_val2;
    
    if(emg_val1 > adc_th[0])
        is_EMG_open = true;
    else
        is_EMG_open = false;
    if(emg_val2 > adc_th[1])
        is_EMG_close = true;
    else
        is_EMG_close = false;

}

int ADC_lib::get_ADC_val(int id)
{
    return adc_val[id];
}

void ADC_lib::set_ADC_th(int id, int threshold)
{
    adc_th[id] = threshold;
}

void ADC_lib::ADC_reset()
{
    adc_val[EMG_PIN_1] = adc_val[EMG_PIN_2] =  0;
    for(int size = 0; size < BUF_SIZE; size++)
    {
        for(int id=0; id < EMG_NUM; id++){
            adc_buf[id][size] = 0;
        }
    }
}

void ADC_lib::cal_ADC_avg(int id)
{
    long val = 0;
    for(int size = 0; size < BUF_SIZE; size++)
    {
        val += (long)adc_buf[id][size];
    }
    adc_val[id] = (short)(val / BUF_SIZE);
    
}

//筋電センサのキャリブレーション
void ADC_lib::EMG_Calibration()
{
    Reset_ADC_th();
    for(int time = 0; time < calib_time; time++)
    {
        //delay(100);
    }
    
}