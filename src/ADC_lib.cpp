#include "ADC_lib.hpp"

void ADC_lib::set_ADC_val(int emg_val1, int emg_val2, int fsr_val1, int fsr_val2)
{

    adc_val[ID_FSR1] = fsr_val1;
    adc_val[ID_FSR2] = fsr_val2;

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

    adc_buf[ID_EMG1][buf_index] = emg_val1;
    adc_buf[ID_EMG2][buf_index] = emg_val2;

    buf_index = (buf_index + 1) % BUF_SIZE;

    cal_ADC_avg(ID_EMG1);
    cal_ADC_avg(ID_EMG2);
    
    if(adc_val[ID_EMG1] > adc_th[ID_EMG1])
        is_EMG_open = true;
    else
        is_EMG_open = false;
    if(adc_val[ID_EMG2] > adc_th[ID_EMG2])
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
    adc_val[ID_EMG1] = adc_val[ID_EMG2] =  0;
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

void ADC_lib::EMG_Calibration()
{
    for(int time = 0; time < BUF_SIZE; time++)
    {
        set_EMG_raw_data(analogRead(ID_EMG1), analogRead(ID_EMG2));
        delay(10);
    }
    set_EMG_base(get_ADC_val(ID_EMG1), get_ADC_val(ID_EMG2));

}
void ADC_lib::set_EMG_raw_data(int emg_val1, int emg_val2)
{
    adc_buf[ID_EMG1][buf_index] = emg_val1;
    adc_buf[ID_EMG2][buf_index] = emg_val2;

    cal_ADC_avg(ID_EMG1);
    cal_ADC_avg(ID_EMG2);

    buf_index++;

    if (buf_index >= BUF_SIZE) {
        buf_index = 0;
    }
}

void ADC_lib::set_EMG_base(int base_1, int base_2)
{
    emg_base[ID_EMG1] = base_1;
    emg_base[ID_EMG2] = base_2;
}