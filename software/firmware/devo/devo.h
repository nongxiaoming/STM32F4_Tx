#ifndef _DEVO_H_
#define _DEVO_H_
#include <stdint.h>

#ifndef HAS_STANDARD_GUI
    #define HAS_STANDARD_GUI 1
#endif
#ifndef HAS_ADVANCED_GUI
    #define HAS_ADVANCED_GUI 1
#endif

void ADC_HW_Init(void);
void ADC_StartCapture(void);

enum {
    TIMER_SOUND = LAST_PRIORITY,
    NUM_MSEC_CALLBACKS,
};
void CLOCK_ClearMsecCallback(int MsecCallback);
uint32_t SOUND_Callback(void);

extern void PROTO_Stubs(int);
// ADC defines
#define NUM_ADC_CHANNELS (INP_HAS_CALIBRATION + 2) //Inputs + Temprature + Voltage
extern const uint8_t adc_chan_sel[NUM_ADC_CHANNELS];
extern volatile uint16_t adc_array_raw[NUM_ADC_CHANNELS];
void ADC_Filter();

#endif
