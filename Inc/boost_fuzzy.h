/*
 * boost_fuzzy.h
 *
 *  Created on: May 22, 2024
 *      Author: prodigytrip
 */

#ifndef INC_BOOST_FUZZY_H_
#define INC_BOOST_FUZZY_H_

#include "stm32f4xx_hal.h"
#include "stdint.h"
#define __IO volatile 
typedef struct fuzzy_t {
	ADC_HandleTypeDef *hadcx;
	TIM_HandleTypeDef *htimx;
	uint8_t TIM_Channel;
	uint32_t PWM_Range;
	__IO int refVoltage;
	__IO int feadbackVoltage;
	__IO int adc_error;
	__IO int Vref;
	__IO int adc_Err;
 	__IO int adc_prevErr;
  	__IO int adc_value;
  	__IO float adc_voltage;
 	 uint32_t pwm_Val;
 	__IO int errChange;
 	__IO int err;
 	__IO int changee;
 	__IO int output;
} fuzzy_t;


#define NE 0
#define NH 1
#define NM 2
#define NS 3
#define NT 4
#define Z  5
#define PT 6
#define PS 7
#define PM 8
#define PH 9
#define PE 10


#define extreme_duty_Change 30
#define high_duty_Change 22
#define medium_duty_Change 14
#define small_duty_Change 8
#define tiny_duty_Change 3


void fuzzy_PWM_Generate(fuzzy_t *fuzzyX);
int return_Output(int error,int errorChange);
uint32_t get_adc_voltage(fuzzy_t *fuzzyX); 

#endif /* INC_BOOST_FUZZY_H_ */