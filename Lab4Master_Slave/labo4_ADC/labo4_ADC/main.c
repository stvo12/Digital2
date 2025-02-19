/*
 * labo4_ADC.c
 *
 * Created: 2/13/2025 6:47:45 PM
 * Author : cssos
 */ 

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include "I2C_SLAVE.h"
#include "ADC.h"

uint8_t buffer = 0;

#define slave_address 0x30

volatile uint8_t adc_value1;

int main(void) {
		cli();  // Deshabilita interrupciones globales
		DDRC &= ~((1 << PORTC4) | (1 << PORTC5)); //Entradas
		PORTC |= (1 << PORTC4) | (1 << PORTC5); //pull up
		DDRC &= ~(1 << PORTC0);
		DDRB |= (1 << DDB5);
	
	ADC_init(128);  // Inicializa ADC con un prescaler de 128
	//DIDR0 |= (ADC0D);
	ADC0();  // Selecciona el canal ADC0
	conversion();  // Inicia la conversión del ADC
	
	I2C_slave_init(slave_address);  // Inicializa I2C en modo esclavo con la dirección definida
	sei();  // Habilita interrupciones globales
	
	while (1) {
				if (buffer == 'P'){
					PORTB |= (1 << DDB5);
					buffer = 0;
				}
	}
}

ISR(ADC_vect) {
	adc_value1 = ADCH;  // Lee el valor del ADC
	conversion();  // Inicia una nueva conversión
	ADCSRA |= (1<<ADIF);
}

ISR(TWI_vect){
	uint8_t estado;
	estado = TWSR & 0xF8;
	switch(estado){
		case 0x60:
		case 0x70:
		TWCR |= (1 << TWINT)|(1 << TWEA)|(1 << TWEN)|(1 << TWIE);
		break;
		case 0x80:
		case 0x90:
		buffer = TWDR;
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE); //limpiar bandera
		break;
		case 0xA8:
		case 0xB8:
		TWDR = adc_value1; //cargar el dato
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE); //enviar
		break;
		default:
		TWCR |= (1 << TWINT) | (1 << TWSTO) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
		break;
		
	}
}