/* 
 * File:   hal_adc.c
 * Author: Ahmed Abd El-Ghafar
 * https://www.linkedin.com/in/ahmedabdelghafarmohammed/
 * Created on March 26, 2021, 7:21 PM
 */

#include "hal_adc.h"

#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static InterruptHandler ADC_InterruptHandler = NULL;       
#endif

static inline void adc_input_channel_port_configuration(const ADC_CHANNEL_SELECT_t channel);
static inline void adc_select_format(const adc_cfg_t *adc);
static inline void configure_the_voltage_reference(const adc_cfg_t *adc);

/**
 * 
 * @param adc
 * @return 
 */
Std_ReturnType ADC_INIT(const adc_cfg_t *adc)
{
    Std_ReturnType ret=E_NOT_OK;
    if(NULL==adc)
    {
        ret=E_NOT_OK;
    }
    else
    {
        /*DISABLE THE ADC*/
        ADC_CONVERSION_DISABLED();
        /*CONFIGURE THE ACQUISITION TIME*/
        ADCON2bits.ACQT=adc->asquisition_time;
        /*CONFIGURE THE CONVERSION CLOCK*/
        ADCON2bits.ADCS=adc->conversion_clock;
        adc_input_channel_port_configuration(adc->adc_channel);
        /*configure the interrupt*/
        #if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        ADC_INTERRUPT_ENABLE();
        ADC_INTERRUPT_CLEAR_FLAG();
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
        if(INTERRUPT_HIGH_PRIORITY==adc->priority){ ADC_HighPrioritySet();}
        else if(INTERRUPT_LOW_PRIORITY==adc->priority){ ADC_LowPrioritySet();}
        else{}
        #endif
       
         ADC_InterruptHandler=adc->ADC_INTERRUPTHANDLER;
        
        #endif
        /*configure the result format*/
        adc_select_format(adc);
        /*CONFIGURE THE VOLTAGE REFERNECE*/
        configure_the_voltage_reference(adc);
        /*ENABLE THE ADC*/
        ADC_CONVERSION_ENABLE();
        ret=E_OK;
    }
    return ret;
}
/**
 * 
 * @param adc
 * @return 
 */
Std_ReturnType ADC_DEINIT(const adc_cfg_t *adc)
{
    Std_ReturnType ret=E_NOT_OK;
    if(NULL==adc)
    {
        ret=E_NOT_OK;
    }
    else
    {
        /*DISABLE THE ADC*/
        ADC_CONVERSION_DISABLED();
        /*configure the interrupt*/
        #if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        ADC_INTERRUPT_DISABLE();
        #endif
        
        ret=E_OK;
    }
    return ret;
}
/**
 * 
 * @param adc
 * @param channel
 * @return 
 */
Std_ReturnType ADC_SELECT_CHANNEL(const adc_cfg_t *adc,ADC_CHANNEL_SELECT_t channel)
{
    Std_ReturnType ret=E_NOT_OK;
    if(NULL==adc)
    {
        ret=E_NOT_OK;
    }
    else
    {
        /*CONFIGURE THE DEFAULT CHANNEL*/
        ADCON0bits.CHS=channel;
        adc_input_channel_port_configuration(channel);
        
        ret=E_OK;
    }
    return ret;
}
/**
 * 
 * @param adc
 * @return 
 */
Std_ReturnType ADC_START_CONVERSION(const adc_cfg_t *adc)
{
    Std_ReturnType ret=E_NOT_OK;
    if(NULL==adc)
    {
        ret=E_NOT_OK;
    }
    else
    {
        ACD_START_CONVERSION();
        ret=E_OK;
    }
    return ret;
}
/**
 * 
 * @param adc
 * @param conversion_status
 * @return 
 */
Std_ReturnType ADC_IS_CONVERSION_DONE(const adc_cfg_t *adc, uint8 *conversion_status)
{
    Std_ReturnType ret=E_NOT_OK;
    if(NULL==adc||NULL==conversion_status)
    {
        ret=E_NOT_OK;
    }
    else
    {
        *conversion_status=(uint8)(!(ADCON0bits.GO_nDONE));
        ret=E_OK;
    }
    return ret;
}
/**
 * 
 * @param adc
 * @param conversion_status
 * @return 
 */
Std_ReturnType ADC_get_conversion_result(const adc_cfg_t *adc, adc_result_t *conversion_result)
{
    Std_ReturnType ret=E_NOT_OK;
    if(NULL==adc||NULL==conversion_result)
    {
        ret=E_NOT_OK;
    }
    else
    {
        switch(adc->result_format)
        {
               case ADC_RESULT_RIGHT:*conversion_result= (adc_result_t)((ADRESH<<8)+ADRESL);break;
               case ADC_RESULT_left:*conversion_result= (adc_result_t)(((ADRESH<<8)+ADRESL) >> 6);break;
               default:*conversion_result= (adc_result_t)((ADRESH<<8)+ADRESL);
                
        }
        ret=E_OK;
    }
    return ret;
}
/**
 * 
 * @param adc
 * @param channel
 * @param conversion_status
 * @return 
 */
Std_ReturnType ADC_get_conversion_blocking(const adc_cfg_t *adc,ADC_CHANNEL_SELECT_t channel ,adc_result_t *conversion_result)
        {
    Std_ReturnType ret=E_NOT_OK;
    uint8 coversion_status=ZERO_INIT;
    if(NULL==adc||NULL==conversion_result)
    {
        ret=E_NOT_OK;
    }
    else
    {
        /*SELECT CHANNEL*/
        ADC_SELECT_CHANNEL(adc,channel);
        /*START CONVERSION*/
        ADC_START_CONVERSION(adc);
        /*CHECK IF THE CONVERSION COMPLETED*/
        
        ret=ADC_IS_CONVERSION_DONE(adc,&coversion_status);
        while(ADCON0bits.GO_nDONE);
        ret = ADC_get_conversion_result(adc, conversion_result);
          
    }
    return ret;
}
/**
 * 
 * @param adc
 * @param channel
 * @param conversion_status
 * @return 
 */
Std_ReturnType ADC_get_conversion_interrupt(const adc_cfg_t *adc,ADC_CHANNEL_SELECT_t channel )
        {
    Std_ReturnType ret=E_NOT_OK;
    uint8 coversion_status=ZERO_INIT;
    if(NULL==adc)
    {
        ret=E_NOT_OK;
    }
    else
    {
        /*SELECT CHANNEL*/
        ADC_SELECT_CHANNEL(adc,channel);
        /*START CONVERSION*/
        ADC_START_CONVERSION(adc);
        
    
    }
    return ret;
}
/**
 * 
 * @param adc
 */
static inline void adc_input_channel_port_configuration(const ADC_CHANNEL_SELECT_t channel)
{
    Std_ReturnType ret=E_NOT_OK;
  
    
        switch(channel)
        {
            case ADC_CHANNEL_AN0: SET_BIT(TRISA,_TRISA_RA0_POSN);break;
                case ADC_CHANNEL_AN1: SET_BIT(TRISA,_TRISA_RA1_POSN);break;
                case ADC_CHANNEL_AN2: SET_BIT(TRISA,_TRISA_RA2_POSN);break;
                case ADC_CHANNEL_AN3: SET_BIT(TRISA,_TRISA_RA3_POSN);break;
                case ADC_CHANNEL_AN4: SET_BIT(TRISA,_TRISA_RA4_POSN);break;
                case ADC_CHANNEL_AN5: SET_BIT(TRISE,_TRISE_RE0_POSN);break;
                case ADC_CHANNEL_AN6: SET_BIT(TRISE,_TRISE_RE1_POSN);break;
                case ADC_CHANNEL_AN7: SET_BIT(TRISE,_TRISE_RE2_POSN);break;
                case ADC_CHANNEL_AN8: SET_BIT(TRISB,_TRISB_RB2_POSN);break;
                case ADC_CHANNEL_AN9: SET_BIT(TRISB,_TRISB_RB3_POSN);break;
                case ADC_CHANNEL_AN10: SET_BIT(TRISB,_TRISB_RB1_POSN);break;
                case ADC_CHANNEL_AN11: SET_BIT(TRISB,_TRISB_RB4_POSN);break;
                case ADC_CHANNEL_AN12: SET_BIT(TRISB,_TRISB_RB0_POSN);break;
                default:;
        }
                
        ret=E_OK;
    
   
}

static inline void adc_select_format(const adc_cfg_t *adc)
{
    Std_ReturnType ret=E_NOT_OK;
    if(NULL==adc)
    {
        ret=E_NOT_OK;
    }
    else
    {
        switch(adc->result_format)
        {
               case ADC_RESULT_RIGHT:ADC_RESULT_RIGHT_FORMAT();break;
               case ADC_RESULT_left:ADC_RESULT_LEFT_FORMAT();break;
            default:ADC_RESULT_RIGHT_FORMAT();
                
        }
                
        ret=E_OK;
    }
}

static inline void configure_the_voltage_reference(const adc_cfg_t *adc)
{
    Std_ReturnType ret=E_NOT_OK;
    if(NULL==adc)
    {
        ret=E_NOT_OK;
    }
    else
    {
        switch(adc->voltage_reference)
        {
               case ADC_VOLTAGE_REFERNCE_ENABLED:ADC_VOLTAGE_REFERENCE_ENABLE();break;
               case ADC_VOLTAGE_REFERNCE_DISABLED:ADC_VOLTAGE_REFERENCE_DISABLE();break;
            default:ADC_VOLTAGE_REFERENCE_DISABLE();
                
        }
                
        ret=E_OK;
    }
}

void ADC_ISR(void)
{
    ADC_INTERRUPT_CLEAR_FLAG();
    if(ADC_InterruptHandler)
    {
        ADC_InterruptHandler();
    }
}