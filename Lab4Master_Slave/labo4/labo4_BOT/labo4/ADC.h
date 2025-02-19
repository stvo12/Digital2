/*
 * ADC.h
 *
 * Created: 2/13/2025 6:40:12 PM
 *  Author: cssos
 */ 


#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>

void ADC_init(uint8_t prescaler);
void ADC0(void);
void ADC1(void);
void ADC2(void);
void ADC3(void);
void ADC4(void);
void ADC5(void);
void ADC6(void);
void ADC7(void);
void conversion(void);




#endif /* ADC_H_ */