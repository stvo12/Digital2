/*
 * Lab1.c
 *
 * Created: 23/01/2025 05:31:30 p. m.
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

uint8_t empezar = 0;
uint8_t hab = 0;
uint8_t Cont1 = 0;
uint8_t Cont2 = 0;
uint8_t Jug1 = 0;
uint8_t Jug2 = 0;
uint8_t Term1 = 0;
uint8_t Term2 = 0;

void Setup(void);
void Salida1(uint8_t Cont1);
void Salida2(uint8_t Cont2);
void Mux(void);

int main(void)
{
    Setup();
	
	sei();	
	
    while (1) 
    {
		if (empezar == 1)
		{
			empezar = 0;
			Nums(5);
			_delay_ms(1000);
			Nums(4);
			_delay_ms(1000);
			Nums(3);
			_delay_ms(1000);
			Nums(2);
			_delay_ms(1000);
			Nums(1);
			_delay_ms(1000);
			Nums(0);
			Cont1 = 0;
			Cont2 = 0;
			hab = 1;
		}
		if (Term1 == 1)
		{
			hab = 0;
			Jug1 = 0x0F;
			Cont2 = 0;
			Nums(1);
		}
		else if (Term2 == 1)
		{
			hab = 0;
			Jug2 = 0x0F;
			Cont1 = 0;
			Nums(2);
		}
		Salida1(Cont1);
		Salida2(Cont2);
		Mux();
    }
}

void Setup(void){
	hab = 0;
	empezar = 0;
	DDRC &= ~((1<<PC0) | (1<<PC1) | (1<<PC2));
	DDRC |= ((1<<PC3) | (1<<PC4) | (1<<PC5) | (1<<PC6));
	DDRB |= ((1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3));
	initDisplay();
	// Habilitar la interrupción de cambio de pin en PC0 (PCINT8) y PC3 (PCINT11)
	PCICR |= (1 << PCIE1);  // Habilitar interrupciones de cambio de pin para el grupo PCINT[14:8]
	PCMSK1 |= (1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10);  // Habilitar interrupciones de cambio de pin para PC0 (PCINT8), PC1 (PCINT9) y PC2 (PCINT10)
}

ISR(PCINT1_vect){
	if (PINC == 0x04){
		empezar = 1;
		Cont1 = 0;
		Cont2 = 0;
		Jug1 = 0;
		Jug2 = 0;
		Term1 = 0;
		Term2 = 0;
	}
	else if ((PINC == 0x02 && hab == 1))
	{
		Cont2++;
	}
	else if ((PINC == 0x01 && hab == 1))
	{
		Cont1++;
	}
	else if ((PINC == 0x03 && hab == 1))
	{
		Cont1++;
		Cont2++;
	}
}

void Salida1(uint8_t Cont1){
	switch(Cont1){
		case 0:
		Jug1 = 0x00;
		break;
		case 1:
		Jug1 = 0x01;
		break;
		case 2:
		Jug1 = 0x02;
		break;
		case 3:
		Jug1 = 0x04;
		break;
		case 4:
		//Jug1 = 0x08;
		Term1 = 1;
		break;
		default:
		Jug1 = 0x00;
	}
}

void Salida2(uint8_t Cont2){
	switch(Cont2){
		case 0:
			Jug2 = 0x00;
			break;
		case 1:
			Jug2 = 0x01;
			break;
		case 2:
			Jug2 = 0x02;
			break;
		case 3:
			Jug2 = 0x04;
			break;
		case 4:
			//Jug2 = 0x08;
			Term2 = 1;
			break;
		default:
			Jug2 = 0x00;
	}
}

void Mux(void){
	_delay_ms(8);
	PORTB = Jug1;
	PORTC |= (1<<PC3);
	PORTC &= ~(1<<PC4);
	_delay_ms(8);
	PORTB = Jug2;
	PORTC |= (1<<PC4);
	PORTC &= ~(1<<PC3);
	
}