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

void initPWM0FastA(uint8_t inverted, uint8_t mode);//invertido = 1, modo (8bits= 0, 9=1, 10=2, ICR=3, OCR=4)
void initPWM0FastB(uint8_t prescaler, uint8_t mode);//prescaler (0=0,1=8,2=64,3=256,4=1024)

void initPWM1FastA(uint8_t inverted, uint8_t mode);//invertido = 1, modo (8bits= 0, 9=1, 10=2, ICR=3, OCR=4)
void initPWM1FastB(uint8_t prescaler, uint8_t mode);//prescaler (0=0,1=8,2=64,3=256,4=1024)

void initPWM2FastA(uint8_t inverted, uint8_t mode);//invertido = 1, modo (8bits= 0, 9=1, 10=2, ICR=3, OCR=4)
void initPWM2FastB(uint8_t prescaler, uint8_t mode);//prescaler (0=0,1=8,2=64,3=256,4=1024)

void updateDutyCycle1A(uint8_t Cycle);
void updateDutyCycle1B(uint8_t Cycle);

void updateDutyCycle2A(uint8_t Cycle);
void updateDutyCycle2B(uint8_t Cycle);

void updateDutyCycle0A(uint8_t Cycle);
void updateDutyCycle0B(uint8_t Cycle);

void initUART(void);
void WriteUART(char Caracter);
void TextUART(char * Texto);

void initDisplay(void);
void Nums(uint8_t Numero);

void lcd_command(char a);
void lcd_write_char(char data);
void lcd_init();
void lcd_setCursor(char c, char f);
void lcd_write_string(char *str);
void lcd_port(char a);

void floatToString(float num, char *str, int precision);
void floatToStringNo(float num, char *str, int precision);

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

//I2C
//iniciar maestro
void InitMasterI2C(unsigned long SCL_CLOCK, uint8_t Prescaler);
//inicio com
void StartMasterI2C(void);
//parar com
void StopMasterI2C(void);
//transm datos, devuelve 0 si slave recibio
uint8_t WriteMasterI2C(uint8_t dato);
//recibir datos por slave a master
uint8_t ReadMasterI2C(uint8_t *buffer, uint8_t ack);
//iniciar slave
void InitSlaveI2C(uint8_t address);

#endif /* FUNCTIONS_H_ */