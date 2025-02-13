/*
 * Functions.c
 *
 * Created: 15/04/2024 11:28:25 p. m.
 *  Author: e-mel
 */ 

#define F_CPU 16000000
#include "Functions.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

// Definir pines (LCD)
#define E (1<<PORTB2);


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

//SPI
void spiInit(Spi_Type sType, Spi_Data_Order sDataOrder, Spi_Clock_Polarity sClockPolarity, Spi_Clock_Phase sClockPhase){
	//PB2=SS, PB3=MOSI, PB4=MISO, PB5=SCK
	if (sType & (1<<MSTR))
	{
		DDRB |= ((1<<DDB3)|(1<<DDB5)|(1<<DDB2));
		DDRB &= ~(1<<DDB4);
		SPCR |= (1<<MSTR);
		
		uint8_t temp = sType & 0b00000111;
		switch(temp){
			case 0: //2
				SPCR &= ~((1<<SPR1)|(1<<SPR0));
				SPSR |= (1<<SPI2X);
			break;
			case 1: //4
				SPCR &= ~((1<<SPR1)|(1<<SPR0));
				SPSR &= ~(1<<SPI2X);
			break;
			case 2: //8
				SPCR |= (1<<SPR0);
				SPCR &= ~(1<<SPR1);
				SPSR |= (1<<SPI2X);
			break;
			case 3: //16
				SPCR |= (1<<SPR0);
				SPCR &= ~(1<<SPR1);
				SPSR &= ~(1<<SPI2X);
			break;
			case 4: //32
				SPCR &= ~(1<<SPR0);
				SPCR |= (1<<SPR1);
				SPSR |= (1<<SPI2X);
			break;
			case 5: //64
				SPCR &= ~(1<<SPR0);
				SPCR |= (1<<SPR1);
				SPSR &= ~(1<<SPI2X);
			break;
			case 6: //128
				SPCR |= ((1<<SPR0)|(1<<SPR1));
				SPSR &= ~(1<<SPI2X);
			break;
		}
	}
	else{
		DDRB |= (1<<DDB4);
		DDRB &= ~((1<<DDB3)|(1<<DDB5)|(1<<DDB2));
		SPCR &= ~(1<<MSTR);
	}
	//EnaSPI, DataOrder, CLKPolar, CLKPhase
	SPCR |= (1<<SPE)|sDataOrder|sClockPolarity|sClockPhase;
}

/*static void spiReceiveWait(){
	while(!(SPSR & (1<<SPIF)));
}*/

void spiWrite(uint8_t dat){
	SPDR = dat;
}

unsigned spiDataReady(){
	if (SPSR & (1<<SPIF))
	return 1;
	else
	return 0;
}

uint8_t spiRead(void){
	while(!(SPSR & (1<<SPIF)));
	return(SPDR);
}