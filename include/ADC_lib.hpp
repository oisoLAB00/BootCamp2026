#pragma once
#define ADC_NUM 2
#define EMG_PIN_1 0
#define EMG_PIN_2 1
#define FSR_PIN_1 2
#define FSR_PIN_2 3

class ADC_input
{
    public :
        ADC_input()
        {
            Reset_val();
            Reset_emg_th();
        }

        ~ADC_input()
        {
            Reset_val();
            Reset_emg_th();
        }

        void set_ADC_input(int* val);
        int get_ADC_val(int id);
        void set_emg_th(int th_ID1, int th_ID2);


    private :
        volatile int adc_val[ADC_NUM];
        int emg_th[ADC_NUM];

        void Reset_val()
        {
            adc_val[EMG_PIN_1] = adc_val[EMG_PIN_2] =  0;
        }

        void Reset_emg_th()
        {
            emg_th[0] = emg_th[1] = 0;
        }
            
};

