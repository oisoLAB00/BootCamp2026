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

void ADC_lib::update() {
    adc_continuous_data_t *result = NULL;
    uint32_t count = 0;

    // DMAバッファからデータを取得
    if (analogContinuousRead(&result, &count, READ_TIMEOUT_MS)) {
        for (int i = 0; i < count; i++) {
            int raw_val = result[i].avg_read_raw;
            int pin = result[i].pin;

            // ピン番号からIDを特定して格納
            int id = -1;
            if (pin == EMG_PIN_1) id = ID_EMG1;
            else if (pin == EMG_PIN_2) id = ID_EMG2;
            else if (pin == FSR_PIN_1) id = ID_FSR1;
            else if (pin == FSR_PIN_2) id = ID_FSR2;

            if (id != -1) {
                if (id == ID_EMG1 || id == ID_EMG2) {
                    // EMGは既存の移動平均ロジックへ
                    adc_buf[id][buf_index] = raw_val;
                } else {
                    // FSRはそのまま格納（または別途平滑化）
                    adc_val[id] = raw_val;
                }
            }
        }
        
        // 既存の平均化・判定ロジック
        buf_index = (buf_index + 1) % BUF_SIZE;
        cal_ADC_avg(ID_EMG1);
        cal_ADC_avg(ID_EMG2);

        // 判定フラグ更新
        is_FSR1 = (adc_val[ID_FSR1] > FSR_TH_VAL);
        is_FSR2 = (adc_val[ID_FSR2] > FSR_TH_VAL);
        is_EMG_open = (adc_val[ID_EMG1] > adc_th[ID_EMG1]);
        is_EMG_close = (adc_val[ID_EMG2] > adc_th[ID_EMG2]);
    }
}