/* 
 * File:   application.c
 * Author: Ahmed.Elghafar
 * https://www.linkedin.com/in/ahmedabdelghafarmohammed/
 * Created on November 28, 2020, 8:43 PM
 */

#include "application.h"
void adc_defaultinterrupthandler(void);
Std_ReturnType ret = E_NOT_OK;
uint8 adc_rss_txt_1[6];
uint8 adc_rss_txt_2[6];

uint8 adc_flag;
uint16 adc_res_1, adc_res_2 ;
uint16 lm35_res_cel_1;
uint16 lm35_res_cel_2;
uint8 adc_req=ZERO_INIT;
adc_cfg_t adc1=
{
#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    .ADC_INTERRUPTHANDLER=adc_defaultinterrupthandler,
#endif
    .adc_channel=ADC_CHANNEL_AN0,
    .asquisition_time=ADC_12_TAD,
    .conversion_clock=ADC_conversion_clock_fosc_div_16,
    .voltage_reference=ADC_VOLTAGE_REFERNCE_DISABLED,
    .result_format=ADC_RESULT_RIGHT
    
};

void adc_defaultinterrupthandler(void)
{
    
            ret=ADC_get_conversion_result(&adc1,&adc_res_1);
            ret=ADC_get_conversion_result(&adc1,&adc_res_2);
          
        
        
    
}

int main()
{ 
    application_intialize();
    ret=lcd_8bit_send_string_pos(&lcd,1,7,"ADC TEST");
    __delay_ms(2000);
    ret=lcd_8bit_send_command(&lcd,_LCD_CLEAR);
    ret=lcd_8bit_send_string_pos(&lcd,1,1,"TEMP1:");
    ret=lcd_8bit_send_string_pos(&lcd,2,1,"TEMP2:");
    
    
    while(1)
    {
        ret=ADC_get_conversion_interrupt(&adc1,ADC_CHANNEL_AN0);
        ret=ADC_get_conversion_interrupt(&adc1,ADC_CHANNEL_AN1);
    
        lm35_res_cel_1=adc_res_1 * 4.88f;
        lm35_res_cel_2=adc_res_2 * 4.88f;
        
        lm35_res_cel_1/=10;
        lm35_res_cel_2/=10;
        
        ret=convert_uint16_to_string(lm35_res_cel_1,adc_rss_txt_1);
        ret=convert_uint16_to_string(lm35_res_cel_2,adc_rss_txt_2);
        
        
        
        ret=lcd_8bit_send_string_pos(&lcd,1,7,adc_rss_txt_1);
        ret=lcd_8bit_send_string_pos(&lcd,2,7,adc_rss_txt_2);
        
        if(lm35_res_cel_1>30)
        {
            ret=dc_motor_move_right(&dc_motor_1);
        }
        else
        {
            ret=dc_motor_stop(&dc_motor_1);
        }
        if(lm35_res_cel_2<5)
        {
            ret=dc_motor_move_right(&dc_motor_2);
        }
        else
        {
            ret=dc_motor_stop(&dc_motor_2);
        }
    }

    
    
    return (EXIT_SUCCESS);
}
void application_intialize(void){
    Std_ReturnType ret = E_NOT_OK;
    ret=ADC_INIT(&adc1);
    ecu_layer_intialize();
}

