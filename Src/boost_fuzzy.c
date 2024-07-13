/*
 * boost_fuzzy.c
 *
 *  Created on: May 22, 2024
 *      Author: prodigytrip
 */
#include "boost_fuzzy.h"
int FuzzyRules[11][11] = {
       {Z,  NT, NS, NM, NM, NH, NH, NE, NE, NE, NE},
       {PT, Z,  NT, NS, NM, NM, NH, NH, NE, NE, NE},
       {PS, PT, Z,  NT, NS, NM, NM, NH, NH, NE, NE},
       {PM, PS, PT, Z,  NT, NS, NM, NM, NH, NH, NE},
       {PM, PM, PS, PT, Z,  NT, NS, NM, NM, NH, NH},
       {PH, PM, PM, PS, PT, Z,  NT, NS, NM, NM, NH},
       {PH, PH, PM, PM, PS, PT, Z,  NT, NS, NM, NM},
       {PE, PH, PH, PM, PM, PS, PT, Z,  NT, NS, NM},
       {PE, PE, PH, PH, PM, PM, PS, PT, Z,  NT, NS},
       {PE, PE, PE, PH, PH, PM, PM, PS, PT, Z,  NT},
       {PE, PE, PE, PE, PH, PH, PM, PM, PS, PT, Z}
   };
uint32_t get_adc_voltage(fuzzy_t *fuzzyX) 
{
    fuzzyX->adc_value = 0;
	for (int i = 0; i < 5; i++) {
		HAL_ADC_Start(fuzzyX->hadcx);
		HAL_ADC_PollForConversion(fuzzyX->hadcx, 1);
		fuzzyX->adc_value  += HAL_ADC_GetValue(fuzzyX->hadcx);
	}
	fuzzyX->adc_value  = fuzzyX->adc_value  / 5;

	fuzzyX->adc_value  = ((float) fuzzyX->adc_value ) * (3300.0f / 4095.0f);
	return (uint32_t) fuzzyX->adc_value ;
}
int return_Output(int error,int errorChange)
{
int out_put = (int)FuzzyRules[error][10-errorChange];
return out_put;
}
void fuzzy_PWM_Generate(fuzzy_t *fuzzyX) {
	get_adc_voltage(fuzzyX);
	fuzzyX->adc_error = fuzzyX->refVoltage - fuzzyX->adc_voltage;
	fuzzyX->errChange = fuzzyX->adc_Err - fuzzyX->adc_prevErr;
	fuzzyX->err = fuzzy_calculator(fuzzyX->adc_error,fuzzyX);
	fuzzyX->changee = fuzzy_calculator(fuzzyX->errChange,fuzzyX);
	fuzzyX->output = return_Output(fuzzyX->err, fuzzyX->changee);
    
	switch (fuzzyX->output) {
	case NE:
		if (fuzzyX->pwm_Val >= fuzzyX->PWM_Range * 0.0625) {
			fuzzyX->pwm_Val = fuzzyX->pwm_Val - fuzzyX->PWM_Range * 0.0625;
		}
		break;
	case NH:
		if (fuzzyX->pwm_Val >= fuzzyX->PWM_Range * 0.037) {
			fuzzyX->pwm_Val = fuzzyX->pwm_Val - fuzzyX->PWM_Range * 0.037;
		}
		break;
	case NM:
		if (fuzzyX->pwm_Val >= fuzzyX->PWM_Range * 0.025) {
			fuzzyX->pwm_Val = fuzzyX->pwm_Val - fuzzyX->PWM_Range * 0.025;
		}
		break;
	case NS:
		if (fuzzyX->pwm_Val >= fuzzyX->PWM_Range * 0.012) {
			fuzzyX->pwm_Val = fuzzyX->pwm_Val - fuzzyX->PWM_Range * 0.012;
		}
		break;
	case NT:
		if (fuzzyX->pwm_Val >= fuzzyX->PWM_Range * 0.005) {
			fuzzyX->pwm_Val = fuzzyX->pwm_Val - fuzzyX->PWM_Range * 0.005;
		}
		break;
	case Z:
		break;
	case PT:
		if (fuzzyX->pwm_Val <= (fuzzyX->PWM_Range * 0.80)) {
			fuzzyX->pwm_Val = fuzzyX->pwm_Val + fuzzyX->PWM_Range * 0.005;
		}
		break;
	case PS:
		if (fuzzyX->pwm_Val <= (fuzzyX->PWM_Range * 0.80)) {
			fuzzyX->pwm_Val = fuzzyX->pwm_Val + fuzzyX->PWM_Range * 0.012;
		}
		break;
	case PM:
		if (fuzzyX->pwm_Val <= (fuzzyX->PWM_Range * 0.80)) {
			fuzzyX->pwm_Val = fuzzyX->pwm_Val + fuzzyX->PWM_Range * 0.025;
		}
		break;
	case PH:
		if (fuzzyX->pwm_Val <= (fuzzyX->PWM_Range * 0.80)) {
			fuzzyX->pwm_Val = fuzzyX->pwm_Val + fuzzyX->PWM_Range * 0.04;
		}
		break;
	case PE:
		if (fuzzyX->pwm_Val <= (fuzzyX->PWM_Range * 0.80)) {
			fuzzyX->pwm_Val = fuzzyX->pwm_Val + fuzzyX->PWM_Range * 0.0625;
		}
		break;
	default:

		break;
	}
	__HAL_TIM_SET_COMPARE(fuzzyX->htimx, fuzzyX->TIM_Channel, fuzzyX->pwm_Val);
	fuzzyX->adc_prevErr = fuzzyX->adc_Err;
}
int fuzzy_calculator(int rawValue,fuzzy_t *fuzzyX)
{

//Zero CTRL
if((rawValue>-(fuzzyX->refVoltage*0.005)) && (rawValue<=(fuzzyX->refVoltage*0.005)))
{
return Z;
}

//Tiny CTRL
else if((rawValue>(fuzzyX->refVoltage*0.005)) && (rawValue<=(fuzzyX->refVoltage*0.10)))
{
return PT;
}
else if((rawValue>-(fuzzyX->refVoltage*0.10)) && (rawValue<=-(fuzzyX->refVoltage*0.005)))
{
return NT;
}

//Small CTRL
else if((rawValue>(fuzzyX->refVoltage*0.10)) && (rawValue<=(fuzzyX->refVoltage*0.20)))
{
return PS;
}
else if((rawValue>-(fuzzyX->refVoltage*0.20)) && (rawValue<=-(fuzzyX->refVoltage*0.10)))
{
return NS;
}

//Medium CTRL
else if((rawValue>(fuzzyX->refVoltage*0.20)) && (rawValue<=(fuzzyX->refVoltage*0.35)))
{
return PM;
}
else if((rawValue>-(fuzzyX->refVoltage*0.35)) && (rawValue<=-(fuzzyX->refVoltage*0.20)))
{
return NM;
}

//High CTRL
else if((rawValue>(fuzzyX->refVoltage*0.35)) && (rawValue<=(fuzzyX->refVoltage*0.45)))
{
return PH;
}
else if((rawValue>-(fuzzyX->refVoltage*0.45)) && (rawValue<=-(fuzzyX->refVoltage*0.35)))
{
return NH;
}
//Extrene CTRL
if(rawValue>(fuzzyX->refVoltage*0.45))
{
return PE;
}
else if((rawValue<=-(fuzzyX->refVoltage*0.45)))
{
return NE;
}
}