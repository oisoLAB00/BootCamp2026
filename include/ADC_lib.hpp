#pragma once
#define ADC_NUM 4
#define BUF_SIZE 100
#define EMG_PIN_1 0
#define EMG_PIN_2 1
#define FSR_PIN_1 2
#define FSR_PIN_2 3

class ADC_lib
{
    public :
        ADC_lib()
        {
            ADC_init();
            Reset_emg_th();
        }

        ~ADC_lib()
        {
            ADC_init();
            Reset_emg_th();
        }

        void set_ADC_val(int emg_val1, int emg_val2, int fsr_val1, int fsr_val2);
        int get_ADC_val(int id);
        void set_emg_th(int th_ID1, int th_ID2);


    private :
        volatile int adc_val[ADC_NUM];
        int emg_th[ADC_NUM];
        int adc_buf[ADC_NUM][BUF_SIZE];

        void ADC_init();

        void Reset_emg_th()
        {
            emg_th[0] = emg_th[1] = 0;
        }

        void cal_ADC_avg();
};

