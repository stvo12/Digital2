/*
 * Lab6AT.c
 *
 * Created: 20/03/2025 08:59:08 p. m.
 * Author : e-mel
 */ 

#define F_CPU 16000000
#include "Functions/Functions.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <stdio.h>

uint16_t A;
uint16_t B;
uint16_t ARR;
uint16_t ABA;
uint16_t IZQ;
uint16_t DER;

uint8_t bufferTX = 0;

void Setup(void);

int main(void)
{
	Setup();
	sei();
    /* Replace with your application code */
    while (1) 
    {
    }
}

void Setup(void){
	initUART();
	DDRC &= ~((1<<PC0) | (1<<PC1) | (1<<PC2) | (1<<PC3) | (1<<PC4) | (1<<PC5));
	PORTC |= ((1<<PC0) | (1<<PC1) | (1<<PC2) | (1<<PC3) | (1<<PC4) | (1<<PC5));
	// Habilitar la interrupción de cambio de pin en PC0 (PCINT8) y PC3 (PCINT11)
	PCICR |= (1 << PCIE1);  // Habilitar interrupciones de cambio de pin para el grupo PCINT[14:8]
	PCMSK1 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3) | (1 << PCINT4) | (1 << PCINT5);  // Habilitar interrupciones de cambio de pin para PC0 (PCINT8), PC1 (PCINT9) y PC2 (PCINT10)
}

ISR(PCINT1_vect){
	uint8_t PINCVar = PINC;
	A = PINCVar & (1 << PINC0);
	_delay_ms(20);
	PINCVar = PINC;
	B = PINCVar & (1 << PINC1);
	PINCVar = PINC;
	ARR = PINCVar & (1 << PINC2);
	PINCVar = PINC;
	ABA = PINCVar & (1 << PINC3);
	PINCVar = PINC;
	IZQ = PINCVar & (1 << PINC4);
	PINCVar = PINC;
	DER = PINCVar & (1 << PINC5);
	
	if (!A){
		TextUART("Boton A\n");
	}
	else if (!B){
		TextUART("Boton B\n");
	}
	else if (!ARR){
		TextUART("Arriba\n");
	}
	else if (!ABA){
		TextUART("Abajo\n");
	}
	else if (!IZQ){
		TextUART("Izquierda\n");
	}
	else if (!DER){
		TextUART("Derecha\n");
	}
	PCIFR |= (1 << PCIF1); 
}

ISR(USART_RX_vect){
	bufferTX = UDR0;
	
	// Reenviar el dato recibido (opcional, tipo echo)
	WriteUART(bufferTX);
	
	// Si detectas fin de línea, podrías hacer algo especial
	if (bufferTX == '\n') {
		TextUART("\n");
	}
}