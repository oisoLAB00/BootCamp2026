#include "ADC_lib.hpp"


void ADC_lib::ADC_init()
{
    pinMode(MODE_SW, INPUT_PULLUP);

    adc_continuous_handle_cfg_t adc_config;

    adc_config.max_store_buf_size = 1024;
    adc_config.conv_frame_size = ADC_DMA_BUF_SIZE;

    ESP_ERROR_CHECK(
        adc_continuous_new_handle(
            &adc_config,
            &adc_handle
        )
    );

    adc_digi_pattern_config_t adc_pattern[ADC_NUM] = {};

    // GPIO34 -> ADC1_CHANNEL_6
    adc_pattern[ID_EMG1].atten = ADC_ATTEN_DB_12;
    adc_pattern[ID_EMG1].channel = ADC_CHANNEL_6;
    adc_pattern[ID_EMG2].unit = ADC_UNIT_1;
    adc_pattern[ID_EMG1].bit_width = SOC_ADC_DIGI_MAX_BITWIDTH;

    // GPIO35 -> ADC1_CHANNEL_7
    adc_pattern[ID_EMG2].atten = ADC_ATTEN_DB_12;
    adc_pattern[ID_EMG2].channel = ADC_CHANNEL_7;
    adc_pattern[ID_EMG2].unit = ADC_UNIT_1;
    adc_pattern[ID_EMG2].bit_width = SOC_ADC_DIGI_MAX_BITWIDTH;

    // GPIO32 -> ADC1_CHANNEL_4
    adc_pattern[ID_FSR1].atten = ADC_ATTEN_DB_12;
    adc_pattern[ID_FSR1].channel = ADC_CHANNEL_4;
    adc_pattern[ID_FSR1].unit = ADC_UNIT_1;
    adc_pattern[ID_FSR1].bit_width = SOC_ADC_DIGI_MAX_BITWIDTH;

    // GPIO33 -> ADC1_CHANNEL_5
    adc_pattern[ID_FSR2].atten = ADC_ATTEN_DB_12;
    adc_pattern[ID_FSR2].channel = ADC_CHANNEL_5;
    adc_pattern[ID_FSR2].unit = ADC_UNIT_1;
    adc_pattern[ID_FSR2].bit_width = SOC_ADC_DIGI_MAX_BITWIDTH;

    adc_continuous_config_t dig_cfg;

    dig_cfg.sample_freq_hz = SAMPLING_FREQ;
    dig_cfg.conv_mode = ADC_CONV_SINGLE_UNIT_1;
    dig_cfg.format = ADC_DIGI_OUTPUT_FORMAT_TYPE1;
    dig_cfg.pattern_num = ADC_NUM;
    dig_cfg.adc_pattern = adc_pattern;

    ESP_ERROR_CHECK(adc_continuous_config(adc_handle, &dig_cfg));

    ESP_ERROR_CHECK(adc_continuous_start(adc_handle));

    ADC_reset();
    Reset_ADC_th();
    set_ADC_th(ID_FSR1, FSR_TH_VAL);
    set_ADC_th(ID_FSR2, FSR_TH_VAL);
    set_ADC_th(ID_EMG1, EMG_TH_OPEN_1);
    set_ADC_th(ID_EMG2, EMG_TH_CLOSE_2);
}


void ADC_lib::set_ADC_val(int emg_val1, int emg_val2, int fsr_val1, int fsr_val2)
{
    adc_val[ID_FSR1] = fsr_val1;
    adc_val[ID_FSR2] = fsr_val2;

    is_FSR1 = (fsr_val1 > FSR_TH_VAL);
    is_FSR2 = (fsr_val2 > FSR_TH_VAL);

    adc_buf[ID_EMG1][buf_index] = emg_val1;
    adc_buf[ID_EMG2][buf_index] = emg_val2;

    buf_index = (buf_index + 1) % BUF_SIZE;

    cal_ADC_avg(ID_EMG1);
    cal_ADC_avg(ID_EMG2);

    is_EMG_open = (adc_val[ID_EMG1] > adc_th[ID_EMG1]);

    is_EMG_close = (adc_val[ID_EMG2] > adc_th[ID_EMG2]);
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
        val += adc_buf[id][size];
    }

    adc_val[id] = (short)(val / BUF_SIZE);
}

void ADC_lib::EMG_Calibration()
{
    for(int time = 0; time < BUF_SIZE; time++)
    {
        set_EMG_raw_data(analogRead(EMG_PIN_1), analogRead(EMG_PIN_2));

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

    if(buf_index >= BUF_SIZE)
    {
        buf_index = 0;
    }
}

void ADC_lib::set_EMG_base( int base_1, int base_2)
{
    emg_base[ID_EMG1] = base_1;
    emg_base[ID_EMG2] = base_2;
}

void ADC_lib::update()
{
    uint8_t result[ADC_DMA_BUF_SIZE];

    uint32_t ret_num = 0;

    esp_err_t ret = adc_continuous_read(adc_handle, result, ADC_DMA_BUF_SIZE, &ret_num, READ_TIMEOUT_MS);

    Serial.println(ret);

    if(ret == ESP_OK)
    {
        for(int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES)
        {
            adc_digi_output_data_t* p = (adc_digi_output_data_t*)&result[i];

            uint32_t channel = p->type1.channel;
            uint32_t data = p->type1.data;

            int id = -1;

            switch(channel)
            {
                case ADC_CHANNEL_6:
                    id = ID_EMG1;
                    break;

                case ADC_CHANNEL_7:
                    id = ID_EMG2;
                    break;

                case ADC_CHANNEL_4:
                    id = ID_FSR1;
                    break;

                case ADC_CHANNEL_5:
                    id = ID_FSR2;
                    break;
            }

            if(id != -1)
            {
                if(id == ID_EMG1 || id == ID_EMG2)
                {
                    adc_buf[id][buf_index] =  data;
                }
                else
                {
                    adc_val[id] = data;
                }
            }
        }

        buf_index = (buf_index + 1) % BUF_SIZE;

        cal_ADC_avg(ID_EMG1);
        cal_ADC_avg(ID_EMG2);

        is_FSR1 = (adc_val[ID_FSR1] > FSR_TH_VAL);
        is_FSR2 = (adc_val[ID_FSR2] > FSR_TH_VAL);

        is_EMG_open = (adc_val[ID_EMG1] > adc_th[ID_EMG1]);
        is_EMG_close =(adc_val[ID_EMG2] > adc_th[ID_EMG2]);
    }
}