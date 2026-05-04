#include "ADC_lib.hpp"

void ADC_lib::set_ADC_val(int emg_val1, int emg_val2, int fsr_val1, int fsr_val2)
{
    adc_val[FSR_PIN_1] = fsr_val1;
    adc_val[FSR_PIN_2] = fsr_val2;

    is_FSR1 = (fsr_val1 > FSR_TH_VAL);
    is_FSR2 = (fsr_val2 > FSR_TH_VAL);

    adc_buf[EMG_PIN_1][buf_index] = emg_val1;
    adc_buf[EMG_PIN_2][buf_index] = emg_val2;

    buf_index = (buf_index + 1) % BUF_SIZE;

    cal_ADC_avg(EMG_PIN_1);
    cal_ADC_avg(EMG_PIN_2);

    is_EMG_open = (adc_val[EMG_PIN_1] > adc_th[EMG_PIN_1]);

    is_EMG_close = (adc_val[EMG_PIN_2] > adc_th[EMG_PIN_2]);
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
    for(int i = 0; i < ADC_NUM; i++)
    {
        adc_val[i] = 0;
    }

    for(int size = 0; size < BUF_SIZE; size++)
    {
        for(int id = 0; id < EMG_NUM; id++)
        {
            adc_buf[id][size] = 0;
        }
    }
}

void ADC_lib::cal_ADC_avg(int id)
{
    long val = 0;
    for(int size = 0; size < BUF_SIZE; size++)
    {
        val += (long)(abs(adc_buf[id][size] - emg_base[id]));
    }
    adc_val[id] = (short)(val / BUF_SIZE);
    
}

//筋電センサのキャリブレーション
void ADC_lib::EMG_Calibration()
{
    for(int time = 0; time < BUF_SIZE; time++)
    {
        //set_ADC_val(analogRead(EMG_PIN_1), analogRead(EMG_PIN_2),analogRead(FSR_PIN_1), analogRead(FSR_PIN_2));
        set_EMG_raw_data(analogRead(EMG_PIN_1), analogRead(EMG_PIN_2));
        delay(10);
    }
    set_EMG_base(get_ADC_val(EMG_PIN_1), get_ADC_val(EMG_PIN_2));

}

void ADC_lib::set_EMG_raw_data(int emg_val1, int emg_val2)
{
    adc_buf[EMG_PIN_1][buf_index] = emg_val1;
    adc_buf[EMG_PIN_2][buf_index] = emg_val2;

    cal_ADC_avg(EMG_PIN_1);
    cal_ADC_avg(EMG_PIN_2);

    buf_index++;

    if(buf_index >= BUF_SIZE)
    {
        buf_index = 0;
    }
}

void ADC_lib::set_EMG_base(int base_1, int base_2)
{
    emg_base[EMG_PIN_1] = base_1;
    emg_base[EMG_PIN_2] = base_2;
}