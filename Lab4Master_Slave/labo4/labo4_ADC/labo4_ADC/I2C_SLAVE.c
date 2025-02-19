/*
 * I2C_SLAVE.c
 *
 * Created: 2/13/2025 7:14:50 PM
 *  Author: cssos
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include "I2C_SLAVE.h"

void I2C_slave_init(uint8_t address){
	DDRC &= ~((1 << DDC4)|(DDC5));
	
	TWAR = address << 1; //direccion que tendra

	TWCR = (1 << TWEA)|(1 << TWEN)|(1 << TWIE); //habilita la ISR

}

