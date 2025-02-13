/*
 * Lab3Master.c
 *
 * Created: 10/02/2025 10:45:13 p. m.
 * Author : e-mel
 */ 

#define F_CPU 16000000
#include "Lab3F/Functions.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

uint8_t ValorSPI1 = 0;
uint8_t ValorSPI2 = 0;
uint8_t ValorSPI3 = 0;
float ValorPot1 = 0;
float ValorPot2 = 0;
uint8_t numASCII = 0;
char buf[20];
uint8_t vacio = 0;

#define BUFF_SIZE 5 
char buff[BUFF_SIZE];
uint8_t indice = 0;
uint8_t received_number = 0; // Para almacenar el número final

void refreshPORT(uint8_t valor);
void Setup();
void floatToString(float num, char *str, int precision);

int main(void)
{
    Setup();
	sei();
	//TextUART("HOLAAAA");
    while (1) 
    {
		spiWrite('P');
		PORTB &= ~(1<<PORTB2);
		ValorSPI1 = spiRead();
		ValorPot1 = ValorSPI1 * 5.0/255;
		floatToString(ValorPot1, buf, 2);
		TextUART("Valor Pote1:");
		WriteUART(10);
		WriteUART(13);
		TextUART(buf);
		WriteUART(10);
		WriteUART(13);
		PORTB |= (1<<PORTB2);
		_delay_ms(250);
		
		spiWrite('C');
		PORTB &= ~(1<<PORTB2);
		ValorSPI2 = spiRead();
		ValorPot2 = ValorSPI2 * 5.0/255;
		floatToString(ValorPot2, buf, 2);
		TextUART("Valor Pote2:");
		WriteUART(10);
		WriteUART(13);
		TextUART(buf);
		WriteUART(10);
		WriteUART(13);
		PORTB |= (1<<PORTB2);
		_delay_ms(250);
	
		PORTB &= ~(1<<PORTB2);
		spiWrite('L');  // Enviar comando de LED
		ValorSPI1 = spiRead();
		ValorPot1 = ValorSPI1 * 5.0/255;
		_delay_ms(250);
		spiWrite(received_number);  // Enviar el valor de los LEDs
		ValorSPI2 = spiRead();
		ValorPot2 = ValorSPI2 * 5.0/255;
		PORTB |= (1<<PORTB2);
		_delay_ms(250);
    }
}

void Setup(){
		
	initUART();
	
	DDRD |= ((1<<DDD2)|(1<<DDD3)|(1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7));
	DDRB |= ((1<<DDB0)|(1<<DDB1));
	DDRB |= (1<<DDB2);
	PORTB |= (1<<PORTB2);
	
	spiInit(SPI_MASTER_OSC16, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_FIRST_EDGE);
}

void refreshPORT(uint8_t valor){
	if (valor & 0b10000000)
	{
		PORTB |= (1<<PORTB1);
	}
	else{
		PORTB &= ~(1<<PORTB1);
	}
	if (valor & 0b01000000)
	{
		PORTB |= (1<<PORTB0);
	}
	else{
		PORTB &= ~(1<<PORTB0);
	}
	if (valor & 0b00100000)
	{
		PORTD |= (1<<PORTD7);
	}
	else{
		PORTD &= ~(1<<PORTD7);
	}
	if (valor & 0b00010000)
	{
		PORTD |= (1<<PORTD6);
	}
	else{
		PORTD &= ~(1<<PORTD6);
	}
	if (valor & 0b00001000)
	{
		PORTD |= (1<<PORTD5);
	}
	else{
		PORTD &= ~(1<<PORTD5);
	}
	if (valor & 0b00000100)
	{
		PORTD |= (1<<PORTD4);
	}
	else{
		PORTD &= ~(1<<PORTD4);
	}
	if (valor & 0b00000010)
	{
		PORTD |= (1<<PORTD3);
	}
	else{
		PORTD &= ~(1<<PORTD3);
	}
	if (valor & 0b00000001)
	{
		PORTD |= (1<<PORTD2);
	}
	else{
		PORTD &= ~(1<<PORTD2);
	}
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

ISR(USART_RX_vect){
	char received = UDR0; // Leer carácter recibido
	UDR0 = received;
	
	if (received == '\r' || received == '\n') { // Si es "Enter"
		buff[indice] = '\0'; // Terminar el string
		received_number = atoi(buff); // Convertir a número entero
		refreshPORT(received_number);
		
		// Enviar número de vuelta para confirmación
		TextUART("\nNumero recibido: ");
		TextUART(buff);
		TextUART("\n");

		// Reiniciar buffer
		indice = 0;
		memset(buff, 0, BUFF_SIZE);
	}
	else if (indice < BUFF_SIZE - 1) { // Almacenar carácter en buffer si hay espacio
		buff[indice++] = received;
	}
}
