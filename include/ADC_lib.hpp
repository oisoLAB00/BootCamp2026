#pragma once

#include <Arduino.h>

#include "esp_adc/adc_continuous.h"
#include "esp_err.h"

#define ADC_NUM 4
#define BUF_SIZE 200

// GPIO
#define EMG_PIN_1 34
#define EMG_PIN_2 35
#define FSR_PIN_1 32
#define FSR_PIN_2 33
#define MODE_SW 22

#define EMG_NUM 2

// Threshold
#define FSR_TH_VAL 100
#define EMG_TH_OPEN_1 2000
#define EMG_TH_CLOSE_2 2000

// ADC ID
#define ID_EMG1 0
#define ID_EMG2 1
#define ID_FSR1 2
#define ID_FSR2 3

// DMA設定
#define CONVERSIONS_PER_PIN 10
#define SAMPLING_FREQ 20000
#define READ_TIMEOUT_MS 10

// DMA Buffer
#define ADC_DMA_BUF_SIZE 256

extern volatile bool is_EMG_close;
extern volatile bool is_EMG_open;
extern volatile bool is_FSR1;
extern volatile bool is_FSR2;

class ADC_lib
{
public:

    ADC_lib()
    {
    }

    ~ADC_lib()
    {
        adc_continuous_stop(adc_handle);
        adc_continuous_deinit(adc_handle);

        ADC_reset();
        Reset_ADC_th();
    }

    void ADC_init();

    void EMG_Calibration();

    void update();

    void set_ADC_val(
        int emg_val1,
        int emg_val2,
        int fsr_val1,
        int fsr_val2
    );

    int get_ADC_val(int id);

private:

    // =========================
    // ADC DMA Handle
    // =========================
    adc_continuous_handle_t adc_handle = NULL;

    // =========================
    // ADC Value
    // =========================
    volatile short adc_val[ADC_NUM];

    int adc_th[ADC_NUM];

    short adc_buf[EMG_NUM][BUF_SIZE];

    short emg_base[ADC_NUM];

    int buf_index{0};

    // =========================
    // ADC GPIO
    // =========================
    uint8_t adc_pins[ADC_NUM] =
    {
        EMG_PIN_1,
        EMG_PIN_2,
        FSR_PIN_1,
        FSR_PIN_2
    };

    // =========================
    // Internal Functions
    // =========================
    void ADC_reset();

    void cal_ADC_avg(int id);

    void set_ADC_th(int id, int threshold);

    void set_EMG_base(int base_1, int base_2);

    void set_EMG_raw_data(int emg_val1, int emg_val2);

    void Reset_ADC_th()
    {
        for(int i = 0; i < ADC_NUM; i++)
        {
            adc_th[i] = 0;
        }
    }
};