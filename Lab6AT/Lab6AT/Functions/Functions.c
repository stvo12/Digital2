/*
 * Functions.c
 *
 * Created: 15/04/2024 11:28:25 p. m.
 *  Author: e-mel
 */ 

#include "Functions.h"
#include <avr/io.h>
#include <stdint.h>

void initADC(uint8_t just, uint8_t canal, uint8_t prescale){
	ADMUX = 0;
	
	// Vref = 5V
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	ADCSRA = 0;
	ADCSRA |= (1<<ADEN); //Enciende ADC
	ADCSRA |= (1<<ADIE); //Int ADC
	
	//Justificación 1(Izq), 0(Der)
	if(just == 1){
		ADMUX |= (1 << ADLAR);
	}
	else{
		ADMUX &= ~(1 << ADLAR);
	}
	
	//Prescaler (cases)
	switch (prescale){
		case 2:
			ADCSRA &= ~((1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2));
			break;
		case 4:
			ADCSRA |= (1 << ADPS1);
			ADCSRA &= ~((1 << ADPS0) | (1 << ADPS2));
			break;
		case 8:
			ADCSRA |= (1 << ADPS0 ) | (1 << ADPS1);
			ADCSRA &= ~(1 << ADPS2);
			break;
		case 16:
			ADCSRA |= (1 << ADPS2);
			ADCSRA &= ~((1 << ADPS0) | (1 << ADPS1));
			break;
		case 32:
			ADCSRA |= (1 << ADPS0 ) | (1 << ADPS2);
			ADCSRA &= ~(1 << ADPS1);
			break;
		case 64:
			ADCSRA |= (1 << ADPS1) | (1 << ADPS2);
			ADCSRA &= ~(1 << ADPS0);
			break;
		case 128:
			ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
			break;
		default:
			ADCSRA &= ~((1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2));
	}
	
	if (canal < 6){
		switch(canal){
			case 0:
				DIDR0 |= (1<<ADC0D);
			break;
			case 1:
				DIDR0 |= (1<<ADC1D);
			break;
			case 2:
				DIDR0 |= (1<<ADC2D);
			break;
			case 3:
				DIDR0 |= (1<<ADC3D);
			break;
			case 4:
				DIDR0 |= (1<<ADC4D);
			break;
			case 5:
				DIDR0 |= (1<<ADC5D);
			break;
			default: DIDR0 &= ~(1<<ADC0D);
		}
	}
}

uint16_t readADC(uint16_t canal){
	ADMUX = (ADMUX & 0xF0) | canal;
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1 << ADSC));
		return ADC;
}

void initPWM0FastA(uint8_t inverted, uint8_t mode){
	DDRD |= (1 << DDD6);
	TCCR0A = 0;
	// Invertido o no
	if(inverted == 1){
		TCCR0A |=  (1 << COM0A1) | (1 << COM0A0);
		//TCCR0A |=  (1 << COM0B1) | (1 << COM0B0);
	}
	else {
		TCCR0A |= (1 << COM0A1);
		//TCCR0A |= (1 << COM0B1);
	}
	
	// Modo
	switch (mode){
		case 0:
		TCCR0A |= (1 << WGM00) | (1<< WGM01);
		break;
		case 1:
		TCCR0A |= (1 << WGM00) | (1 << WGM01);
		break;
		
		default:
		TCCR0A |= (1 << WGM00) | (1 << WGM01);
	}
	
	//ICR1H = 0x04;
	//ICR1L = 0XE1;
}

void initPWM0FastB(uint8_t prescaler, uint8_t mode){
	TCCR0B = 0;
	// Prescaler
	switch (prescaler){
		case 0:
		TCCR0B |= (1 << CS00);
		break;
		case 1:
		TCCR0B |= (1 << CS01);
		break;
		case 2:
		TCCR0B |= (1 << CS00) | (1 << CS01);
		break;
		case 3:
		TCCR0B |= (1 << CS02);
		break;
		case 4:
		TCCR0B |= (1 << CS00) | (1 << CS02);;
		break;
		default:
		TCCR0B |= (1 << CS00);
	}
	// Modo
	switch (mode){
		case 0:
		TCCR0B &= ~(1 << WGM02);
		break;
		case 1:
		TCCR0B |= (1 << WGM02);
		break;
		default:
		TCCR0B &= ~(1 << WGM02);
	}
}

void initPWM1FastA(uint8_t inverted, uint8_t mode){
	DDRB |= (1 << DDB1);
	DDRB |= (1 << DDB2);
	TCCR1A = 0;
	// Invertido o no
	if(inverted == 1){
		TCCR1A |=  (1 << COM1A1) | (1 << COM1A0);
		TCCR1A |=  (1 << COM1B1) | (1 << COM1B0);
	}
	else {
		TCCR1A |= (1 << COM1A1);
		TCCR1A |= (1 << COM1B1);
	}
	
	// Modo
	switch (mode){
		case 0:
			TCCR1A |= (1 << WGM10);
		break;
		case 1:
			TCCR1A |= (1 << WGM11);
		break;
		case 2:
			TCCR1A |= (1 << WGM10) | (1 << WGM11);
		break;
		case 3:
			TCCR1A |= (1 << WGM10);
		break;
		case 4:
			TCCR1A |= (1 << WGM10) | (1 << WGM11);
		break;
		
		default:
			TCCR1A |= (1 << WGM10);
	}
	
	//ICR1H = 0x04;
	//ICR1L = 0XE1;
}

void initPWM1FastB(uint8_t prescaler, uint8_t mode){
	TCCR1B = 0;
	// Prescaler
	switch (prescaler){
		case 0:
			TCCR1B |= (1 << CS10);
		break;
		case 1:
			TCCR1B |= (1 << CS11);
		break;
		case 2:
			TCCR1B |= (1 << CS10) | (1 << CS11);
		break;
		case 3:
			TCCR1B |= (1 << CS12);
		break;
		case 4:
			TCCR1B |= (1 << CS10) | (1 << CS12);
		break;
		default:
			TCCR1B |= (1 << CS10);
	}
	// Modo
	switch (mode){
		case 0:
			TCCR1B |= (1 << WGM12);
		break;
		case 1:
			TCCR1B |= (1 << WGM12);
		break;
		case 2:
			TCCR1B |= (1 << WGM12);
		break;
		case 3:
			TCCR1B |= (1 << WGM12) | (1 << WGM13);
		break;
		case 4:
			TCCR1B |= (1 << WGM12) | (1 << WGM13);
		break;
		default:
			TCCR1B |= (1 << WGM12);
	}
}

void initPWM2FastA(uint8_t inverted, uint8_t mode){
	DDRD |= (1 << DDD3);
	DDRB |= (1 << DDB3);
	TCCR2A = 0;
	// Invertido o no
	if(inverted == 1){
		TCCR2A |=  (1 << COM2A1) | (1 << COM2A0);
		TCCR2A |=  (1 << COM2B1) | (1 << COM2B0);
	}
	else {
		TCCR2A |= (1 << COM2A1);
		TCCR2A |= (1 << COM2B1);
	}
	
	// Modo
	switch (mode){
		case 0:
		TCCR2A |= (1 << WGM20) | (1<< WGM21);
		break;
		case 1:
		TCCR2A |= (1 << WGM20) | (1 << WGM21);
		break;
		
		default:
		TCCR2A |= (1 << WGM20) | (1 << WGM21);
	}
	
	//ICR1H = 0x04;
	//ICR1L = 0XE1;
}

void initPWM2FastB(uint8_t prescaler, uint8_t mode){
	TCCR2B = 0;
	// Prescaler
	switch (prescaler){
		case 0:
		TCCR2B |= (1 << CS20);
		break;
		case 1:
		TCCR2B |= (1 << CS21);
		break;
		case 2:
		TCCR2B |= (1 << CS22);
		break;
		case 3:
		TCCR2B |= (1 << CS22) | (1 << CS21);
		break;
		case 4:
		TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22);
		break;
		default:
		TCCR2B |= (1 << CS20);
	}
	// Modo
	switch (mode){
		case 0:
		TCCR2B &= ~(1 << WGM22);
		break;
		case 1:
		TCCR2B |= (1 << WGM22);
		break;
		default:
		TCCR2B &= ~(1 << WGM22);
	}
}

void updateDutyCycle1A(uint8_t Cycle){
	OCR1A = Cycle;
}

void updateDutyCycle1B(uint8_t Cycle){
	OCR1B = Cycle;
}

void updateDutyCycle2A(uint8_t Cycle){
	OCR2A = Cycle;
}

void updateDutyCycle2B(uint8_t Cycle){
	OCR2B = Cycle;
}

void updateDutyCycle0A(uint8_t Cycle){
	OCR0A = Cycle;
}

void updateDutyCycle0B(uint8_t Cycle){
	OCR0B = Cycle;
}

void initUART(void){
	//rx y tx como entrada y salida
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	
	UCSR0A = 0;
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	
	UCSR0C = 0;
	//asincrono, sin paridad, 1 stop, 8 caract
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	//valor baud y velocidad(9600)
	UBRR0 = 103;
}

void WriteUART(char Caracter){
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = Caracter;
}

void TextUART(char * Texto){
	uint8_t i;
	for (i=0; Texto[i]!= '\0'; i++){
		while (!(UCSR0A & (1<<UDRE0)) );
		UDR0 = Texto[i];
	}
}

void initDisplay(void){
	// Puerto D como salida
	DDRD = 0xFF;
	PORTD = 0;
	// Utilizar Rx y Tx
	UCSR0B = 0;
}

void Nums(uint8_t Numero){
	switch(Numero){
		case 0:
			PORTD = 0x3F;
			break;
		case 1:
			PORTD = 0x06;
			break;
		case 2:
			PORTD = 0x5B;
			break;
		case 3:
			PORTD = 0x4F;
			break;
		case 4:
			PORTD = 0x66;
			break;
		case 5:
			PORTD = 0x6D;
			break;
		case 6:
			PORTD = 0x7D;
			break;
		case 7:
			PORTD = 0x07;
			break;
		case 8:
			PORTD = 0x7F;
			break;
		case 9:
			PORTD = 0x6F;
			break;
		default:
			PORTD = 0x00;
	}
}