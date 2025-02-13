/*
 * Lab3Slave.c
 *
 * Created: 06/02/2025 07:29:23 p. m.
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
#include <string.h> 

uint8_t ValorSPI = 0;
uint8_t ValorADC = 0;
uint8_t Pot1 = 0;
uint8_t Pot2 = 0;
uint8_t spiValor = 0;
char buff_SPI[3] = {0};
uint8_t bufferTX = 0;

#define BUFF_SIZE 4  // Tamaño máximo del número (máximo 3 dígitos + '\0')
char buff[BUFF_SIZE];
uint8_t indice = 0;
uint8_t received_number = 0; // Para almacenar el número final

uint8_t LED = 0;

void refreshPORT(uint8_t valor);
void Setup();


int main(void)
{
	Setup();
	sei();
	//TextUART("HOLAAAAAAAAAA");
    /* Replace with your application code */
    while (1) 
    {
		//TextUART("HOLAAAA");
    }
}

void Setup(){
	
	initADC(1, 7, 128);
	_delay_ms(100);
	initADC(1, 5, 128);
	_delay_ms(100);
	
	initUART();
	
	DDRD |= ((1<<DDD2)|(1<<DDD3)|(1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7));
	DDRB |= ((1<<DDB0)|(1<<DDB1));
	
	spiInit(SPI_SLAVE_SS, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_FIRST_EDGE);
	SPCR |= (1<<SPIE);
	
	//DDRB &= ((1<<DDB2)|(1<<DDB3)|(1<<DDB4));
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

ISR(ADC_vect){
	ValorADC =  ADCH;
	ADCSRA |= (1<<ADIF);
}

ISR(SPI_STC_vect){
	PORTD |= (1<<PORTD2);
	spiValor = SPDR;
	PORTD &= ~(1<<PORTD2);
	buff_SPI[0] = spiValor;
	//TextUART(buff_SPI);
	if (LED == 1)
	{
		buff_SPI[0] = spiValor;
		TextUART("LED\n");
		TextUART(buff_SPI);
		LED = 0;
		refreshPORT(spiValor);
		SPDR = Pot2;
	}
	if (spiValor == 'P')
	{
		readADC(5);
		Pot1 = ValorADC;
		spiWrite(Pot1);
	}
	else if (spiValor == 'C')
	{
		readADC(7);
		Pot2 = ValorADC;
		spiWrite(Pot2);
	}
	else if (spiValor == 'L'){
		LED = 1;
		SPDR = Pot1;
	}
}

ISR(USART_RX_vect){
	/*bufferTX = UDR0;
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = bufferTX;
	buff[0] = bufferTX;
	TextUART(buff);*/
	/*char received = UDR0; // Leer carácter recibido
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
	}*/
}