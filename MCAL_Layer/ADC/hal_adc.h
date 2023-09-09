/* 
 * File:   hal_adc.h
 * Author: Ahmed Abd El-Ghafar
 * https://www.linkedin.com/in/ahmedabdelghafarmohammed/
 * Created on March 26, 2021, 7:21 PM
 */

#ifndef HAL_ADC_H
#define	HAL_ADC_H

/* ----------------- Includes -----------------*/
#include "pic18f4620.h"
#include "../mcal_std_types.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/Interrupt/mcal_internal_interrupt.h"
#include "hal_adc_cfg.h"

/* ----------------- Macro Declarations -----------------*/
/*if you choose AN9 so AN0 TO AN9 IS ANALOG*/
#define ADC_AN0_ANALOG_FUNCTIONALITY        0x0E
#define ADC_AN1_ANALOG_FUNCTIONALITY        0x0D
#define ADC_AN2_ANALOG_FUNCTIONALITY        0x0C
#define ADC_AN3_ANALOG_FUNCTIONALITY        0x0B
#define ADC_AN4_ANALOG_FUNCTIONALITY        0x0A
#define ADC_AN5_ANALOG_FUNCTIONALITY        0x09
#define ADC_AN6_ANALOG_FUNCTIONALITY        0x08
#define ADC_AN7_ANALOG_FUNCTIONALITY        0x07
#define ADC_AN8_ANALOG_FUNCTIONALITY        0x06
#define ADC_AN9_ANALOG_FUNCTIONALITY        0x05
#define ADC_AN10_ANALOG_FUNCTIONALITY       0x04
#define ADC_AN11_ANALOG_FUNCTIONALITY       0x03
#define ADC_AN12_ANALOG_FUNCTIONALITY       0x02
#define ADC_ALL_ANALOG_FUNCTIONALITY        0x00
#define ADC_ALL_DIGITAL_FUNCTIONALITY       0x0F

#define ADC_RESULT_RIGHT        0x01
#define ADC_RESULT_left         0x00

#define ADC_VOLTAGE_REFERNCE_ENABLED         0X01
#define ADC_VOLTAGE_REFERNCE_DISABLED        0X00

#define ADC_CONVERSION_COMPLETED              1
#define ADC_CONVERSION_INPROGRESS             0




/* ----------------- Macro Functions Declarations -----------------*/
/*A/D CONVVERSION STATUS:A/D CONVVERSION IN PROGRESS/A/D IDLE*/
#define ADC_CONVERSION_STATUS()         (ADCON0bits.GO_nDONE)
/*START THE ANALOG TO DIGITAL CONVERSION*/
#define ACD_START_CONVERSION()            (ADCON0bits.GODONE=1)
/**@brief ANALOGTO DIGITAL CONTROL
 * ENABLE THE ANALOG TO DIGITAL
 * DISABLE THE ANALOG TO DIGITAL
 */
#define ADC_CONVERSION_ENABLE()             (ADCON0bits.ADON=1)
#define ADC_CONVERSION_DISABLED()           (ADCON0bits.ADON=0)
/*@BRIEF 
 VOLTAGE REFRENCE CONFIGURATION*/
#define ADC_VOLTAGE_REFERENCE_ENABLE()    do{ADCON1bits.VCFG0=1;\
                                           ADCON1bits.VCFG1=1;\
                                           }while(0);

#define ADC_VOLTAGE_REFERENCE_DISABLE()    do{ADCON1bits.VCFG0=0;\
                                           ADCON1bits.VCFG1=0;\
                                           }while(0);
                                            
#define ADC_ANALOG_DIGITAL_PORT_CONFIG(_config)       (ADCON1bits.PCFG=_config)

#define ADC_RESULT_RIGHT_FORMAT()           (ADCON2bits.ADFM=1)
#define ADC_RESULT_LEFT_FORMAT()            (ADCON2bits.ADFM=0)

/* ----------------- Data Type Declarations -----------------*/
typedef enum
{
    ADC_CHANNEL_AN0,
    ADC_CHANNEL_AN1,   
    ADC_CHANNEL_AN2,
    ADC_CHANNEL_AN3,
    ADC_CHANNEL_AN4,
    ADC_CHANNEL_AN5,   
    ADC_CHANNEL_AN6,
    ADC_CHANNEL_AN7,
    ADC_CHANNEL_AN8,
    ADC_CHANNEL_AN9,
    ADC_CHANNEL_AN10,
    ADC_CHANNEL_AN11,
    ADC_CHANNEL_AN12,
        
}ADC_CHANNEL_SELECT_t;

typedef enum 
{
    ADC_0_TAD,
    ADC_2_TAD,
    ADC_4_TAD,
    ADC_6_TAD, 
    ADC_8_TAD,
    ADC_12_TAD,
    ADC_16_TAD,
    ADC_20_TAD,         
}adc_asquisition_time_t;

typedef enum 
{
    ADC_conversion_clock_fosc_div_2,
    ADC_conversion_clock_fosc_div_8,
    ADC_conversion_clock_fosc_div_32,
    ADC_conversion_clock_fosc_div_frc, 
    ADC_conversion_clock_fosc_div_4,
    ADC_conversion_clock_fosc_div_16,
    ADC_conversion_clock_fosc_div_64,
}adc_conversion_clock_t;

typedef struct 
{
    #if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void(*ADC_INTERRUPTHANDLER)(void);
    interrupt_priority_cfg priority;
    #endif
    adc_asquisition_time_t asquisition_time;
    adc_conversion_clock_t conversion_clock;
    ADC_CHANNEL_SELECT_t   adc_channel;
    uint8 voltage_reference : 1;
    uint8 result_format :1;
    uint8 adc_resrved:6;
}adc_cfg_t;
typedef uint16 adc_result_t;
/* ----------------- Software Interfaces Declarations -----------------*/

Std_ReturnType ADC_INIT(const adc_cfg_t *adc);
Std_ReturnType ADC_DEINIT(const adc_cfg_t *adc);
Std_ReturnType ADC_SELECT_CHANNEL(const adc_cfg_t *adc,ADC_CHANNEL_SELECT_t channel);
Std_ReturnType ADC_START_CONVERSION(const adc_cfg_t *adc);
Std_ReturnType ADC_IS_CONVERSION_DONE(const adc_cfg_t *adc, uint8 *conversion_status);
Std_ReturnType ADC_get_conversion_result(const adc_cfg_t *adc, adc_result_t *conversion_result);
Std_ReturnType ADC_get_conversion_blocking(const adc_cfg_t *adc,ADC_CHANNEL_SELECT_t channel ,adc_result_t *conversion_status);
Std_ReturnType ADC_get_conversion_interrupt(const adc_cfg_t *adc,ADC_CHANNEL_SELECT_t channel );


#endif	/* HAL_ADC_H */
