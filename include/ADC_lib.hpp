#pragma once

#include <Arduino.h>
#define ADC_NUM 4
#define BUF_SIZE 300
#define EMG_PIN_1 0
#define EMG_PIN_2 1
#define FSR_PIN_1 2
#define FSR_PIN_2 3
#define FSR_TH_VAL 100
#define EMG_NUM 2

#define EMG_TH_CLOSE 250 //ID1 (0)
#define EMG_TH_OPEN 200 //ID2 (1)

extern bool is_calibed;

extern bool is_EMG_close;
extern bool is_EMG_open;
extern bool is_FSR1;
extern bool is_FSR2;

class ADC_lib
{
    public :
        ADC_lib()
        {
            //ADC_reset();
            //Reset_emg_th();
        }

        ~ADC_lib()
        {
            ADC_reset();
            Reset_ADC_th();
        }

        void ADC_init()
        {
            ADC_reset();
            Reset_ADC_th();
            set_ADC_th(FSR_PIN_1, FSR_TH_VAL);
            set_ADC_th(FSR_PIN_2, FSR_TH_VAL);
            set_ADC_th(EMG_PIN_1, EMG_TH_OPEN);
            set_ADC_th(EMG_PIN_2, EMG_TH_CLOSE);
        }

        void EMG_Calibration();

        void set_ADC_val(int emg_val1, int emg_val2, int fsr_val1, int fsr_val2);
        int get_ADC_val(int id);
        int get_ADC_th(int id)
        {
            return adc_th[id];
        }
        void set_ADC_th(int id, int threshold);
        


    private :
        volatile short adc_val[ADC_NUM];
        short emg_base[ADC_NUM];
        int adc_th[ADC_NUM];
        short adc_buf[EMG_NUM][BUF_SIZE];
        const int calib_time = 100;

        void ADC_reset();
        void cal_ADC_avg(int id);
        void Reset_ADC_th()
        {
            for(int i =0; i < ADC_NUM ; i++)
            {
                adc_th[i] = 0;
            }
        }
        
        void set_EMG_base(int base_1, int base_2);
        void set_EMG_raw_data(int emg_val1, int emg_val2);
};

