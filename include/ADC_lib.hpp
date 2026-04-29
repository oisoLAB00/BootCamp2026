#pragma once
#include <Arduino.h>

#define ADC_NUM 4
#define BUF_SIZE 200
#define EMG_PIN_1 34
#define EMG_PIN_2 35
#define FSR_PIN_1 32
#define FSR_PIN_2 33
#define MODE_SW 22
#define EMG_NUM 2

#define FSR_TH_VAL 100
#define EMG_TH_OPEN_1 2000
#define EMG_TH_CLOSE_2 2000

#define ID_EMG1 0
#define ID_EMG2 1
#define ID_FSR1 2
#define ID_FSR2 3

//DMA設定
#define CONVERSIONS_PER_PIN 10  // 1回の読み出しで各ピン何回サンプリングするか
#define SAMPLING_FREQ 20000     // サンプリング周波数 20k[Hz]
#define READ_TIMEOUT_MS 10

extern volatile bool is_EMG_close;
extern volatile bool is_EMG_open;
extern volatile bool is_FSR1;
extern volatile bool is_FSR2;

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
            /*
            pinMode(EMG_PIN_1, INPUT);
            pinMode(EMG_PIN_2, INPUT);
            pinMode(FSR_PIN_1, INPUT);
            pinMode(FSR_PIN_2, INPUT);
            */
            analogContinuousInit(adc_pins, ADC_NUM, CONVERSIONS_PER_PIN, SAMPLING_FREQ, NULL);
            analogContinuousSetAtten(ADC_ATTEN_DB_12);
            analogContinuousStart();
            pinMode(MODE_SW, INPUT_PULLUP);
            ADC_reset();
            Reset_ADC_th();
            set_ADC_th(ID_FSR1, FSR_TH_VAL);
            set_ADC_th(ID_FSR2, FSR_TH_VAL);
            set_ADC_th(ID_EMG1, EMG_TH_OPEN_1);
            set_ADC_th(ID_EMG2, EMG_TH_CLOSE_2);
        }

        void EMG_Calibration();
        void update();
        void set_ADC_val(int emg_val1, int emg_val2, int fsr_val1, int fsr_val2);
        int get_ADC_val(int id);
        


    private :
        volatile short adc_val[ADC_NUM];
        int adc_th[ADC_NUM];
        short adc_buf[EMG_NUM][BUF_SIZE];
        short emg_base[ADC_NUM];
        int buf_index{0};
        uint8_t adc_pins[ADC_NUM] = {EMG_PIN_1, EMG_PIN_2, FSR_PIN_1, FSR_PIN_2};

        void ADC_reset();
        void cal_ADC_avg(int id);
        void Reset_ADC_th()
        {
            for(int i =0; i < ADC_NUM ; i++)
            {
                adc_th[i] = 0;
            }
        }
        void set_ADC_th(int id, int threshold);
        void set_EMG_base(int base_1, int base_2);
        void set_EMG_raw_data(int emg_val1, int emg_val2);
};

