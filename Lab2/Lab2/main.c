/*
 * Lab2.c
 *
 * Created: 30/01/2025 06:01:36 p. m.
 * Author : e-mel
 */ 

#define F_CPU 16000000
#include "Functions/Functions.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t ValorADC = 0;
uint8_t Pot1 = 0;
uint8_t Pot2 = 0;
float ValorPot1 = 0;
float ValorPot2 = 0;
char buf[20];
uint8_t bufferTX = 0;
uint8_t bufferRX = 0;
uint8_t Contador = 0;

void Setup(void);
void floatToString(float num, char *str, int precision);

int main(void)
{
    
	Setup();
	lcd_setCursor(1,1);
	lcd_write_string(" S1:   S2:  S3");
	lcd_setCursor(1,2);
	lcd_write_string("    V     V ");
	sei();
	WriteUART(10);
	WriteUART(13);
	TextUART("(+) incrementar contador");
	WriteUART(10);
	WriteUART(13);
	TextUART("(-) decrementar contador");
	WriteUART(10);
	WriteUART(13);
	
    while (1) 
    {
		readADC(5);
		Pot1 = ValorADC;
		ValorPot1 = Pot1 * 5.0/255;
		floatToString(ValorPot1, buf, 2);
		lcd_setCursor(1,2);
		lcd_write_string(buf);
		_delay_ms(50);
		readADC(7);
		Pot2 = ValorADC;
		ValorPot2 = Pot2 * 5.0/255;
		floatToString(ValorPot2, buf, 2);
		lcd_setCursor(7,2);
		lcd_write_string(buf);
		_delay_ms(50);
		itoa(Contador, buf, 10);
		if (Contador < 10)
		{
			lcd_setCursor(13,2);
			lcd_write_string("0");
			lcd_setCursor(14,2);
			lcd_write_string(buf);
		}
		else if (Contador >= 10)
		{
			lcd_setCursor(13,2);
			lcd_write_string(buf);
		}
		_delay_ms(50);
    }
}


void Setup(void){
	lcd_init();
	
	initADC(1, 7, 128);
	_delay_ms(100);
	initADC(1, 5, 128);
	_delay_ms(100);
	
	initUART();
}

void floatToString(float num, char *str, int precision) {
	int intPart = (int)num;  // Parte entera
	float decimalPart = num - intPart;  // Parte decimal

	// Convertir parte entera con itoa()
	itoa(intPart, str, 10);

	// Agregar punto decimal
	int i = 0;
	while (str[i] != '\0') i++;  // Buscar el final de la cadena
	str[i++] = '.';

	// Convertir parte decimal manualmente
	for (int j = 0; j < precision; j++) {
		decimalPart *= 10;
		int decDigit = (int)decimalPart;
		str[i++] = decDigit + '0';
		decimalPart -= decDigit;
	}

	str[i] = '\0';  // Terminar la cadena
}

ISR(ADC_vect){
	ValorADC =  ADCH;
	ADCSRA |= (1<<ADIF);
}

ISR(USART_RX_vect){
	bufferTX = UDR0;
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = bufferTX;
	if (bufferTX == 43)
	{
		Contador++;
	}
	else if (bufferTX == 45)
	{
		Contador--;
	}
}