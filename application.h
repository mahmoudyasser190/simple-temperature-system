/* 
 * File:   application.h
 * Author: Ahmed.Elghafar
 * https://www.linkedin.com/in/ahmedabdelghafarmohammed/
 * Created on November 28, 2020, 9:53 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

/* Section : Includes */
#include "ECU_Layer/ecu_layer_init.h"
#include "MCAL_Layer/ADC/hal_adc.h"

/* Section: Macro Declarations */

/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */
extern keypad_t keypad_obj;
extern chr_lcd_8bit_t lcd;
extern dc_motor_t dc_motor_1;
extern dc_motor_t dc_motor_2;
/* Section: Function Declarations */
void application_intialize(void);

#endif	/* APPLICATION_H */

