/* 
 * File:   mcal_internal_interrupt.h
 * Author: Ahmed.Elghafar
 * https://www.linkedin.com/in/ahmedabdelghafarmohammed/
 * Created on February 14, 2021, 11:52 PM
 */

#ifndef MCAL_INTERNAL_INTERRUPT_H
#define	MCAL_INTERNAL_INTERRUPT_H

/* ----------------- Includes -----------------*/
#include "mcal_interrupt_config.h"

/* ----------------- Macro Declarations -----------------*/

/* ----------------- Macro Functions Declarations -----------------*/
#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
/*THIS ROUTINE CLEARS THE INTERRUPT ENABLE FOR ADC MODULE*/
#define ADC_INTERRUPT_DISABLE()             (PIE1bits.ADIE=0)
/*THIS ROUTINE SETS THE INTERRUPT ENABLE FOR ADC MODULE*/
#define ADC_INTERRUPT_ENABLE()              (PIE1bits.ADIE=1)
/*THIS ROUTINE CLEARS THE FLAG FOR ADC MODULE*/
#define ADC_INTERRUPT_CLEAR_FLAG()          (PIR1bits.ADIF=0)   
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
/* This routine set the INT1 External Interrupt Priority to be High priority */
#define ADC_HighPrioritySet()           (IPR1bits.ADIP = 1)
/* This routine set the INT1 External Interrupt Priority to be Low priority */
#define ADC_LowPrioritySet()            (IPR1bits.ADIP = 0)
/* This routine set the INT2 External Interrupt Priority to be High priority */

#endif
#endif

/* ----------------- Data Type Declarations -----------------*/
typedef void (*InterruptHandler)(void);
/* ----------------- Software Interfaces Declarations -----------------*/

#endif	/* MCAL_INTERNAL_INTERRUPT_H */
