/*
 * Functions.h
 *
 * Created: 15/04/2024 11:12:17 p. m.
 *  Author: e-mel
 */ 


#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <avr/io.h>
#include <stdint.h>

#define E (1<<PORTB2);

void initADC(uint8_t just, uint8_t canal, uint8_t prescale);
uint16_t readADC(uint16_t canal);


void initPWM0(void);

void initUART(void);
void WriteUART(char Caracter);
void TextUART(char * Texto);

//SPI
typedef enum{
	SPI_MASTER_OSC2 = 0b01010000,
	SPI_MASTER_OSC4 = 0b01010001,
	SPI_MASTER_OSC8 = 0b01010010,
	SPI_MASTER_OSC16 = 0b01010011,
	SPI_MASTER_OSC32 = 0b01010100,
	SPI_MASTER_OSC64 = 0b01010101,
	SPI_MASTER_OSC128 = 0b01010110,
	SPI_SLAVE_SS = 0b01000000
	}Spi_Type;
	
typedef enum{
	SPI_DATA_ORDER_MSB = 0b00000000,
	SPI_DATA_ORDER_LSB = 0b00100000
	}Spi_Data_Order;
	
typedef enum{
	SPI_CLOCK_IDLE_HIGH = 0b00001000,
	SPI_CLOCK_IDLE_LOW = 0b00000000
	}Spi_Clock_Polarity;
	
typedef enum{
	SPI_CLOCK_FIRST_EDGE = 0b00000000,
	SPI_CLOCK_LAST_EDGE = 0b00000100
	}Spi_Clock_Phase;

void spiInit(Spi_Type, Spi_Data_Order, Spi_Clock_Polarity, Spi_Clock_Phase);
void spiWrite(uint8_t dat);
//static void spiReceiveWait();
unsigned spiDataReady();
uint8_t spiRead(void);

#endif /* FUNCTIONS_H_ */