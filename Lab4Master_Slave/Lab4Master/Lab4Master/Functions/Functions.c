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
#include <stdlib.h>

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

void initPWM0FastA(uint8_t inverted, uint8_t mode){
	DDRD |= (1 << DDD6);
	TCCR0A = 0;
	// Invertido o no
	if(inverted == 1){
		TCCR0A |=  (1 << COM0A1) | (1 << COM0A0);
		//TCCR0A |=  (1 << COM0B1) | (1 << COM0B0);
	}
	else {
		TCCR0A |= (1 << COM0A1);
		//TCCR0A |= (1 << COM0B1);
	}
	
	// Modo
	switch (mode){
		case 0:
		TCCR0A |= (1 << WGM00) | (1<< WGM01);
		break;
		case 1:
		TCCR0A |= (1 << WGM00) | (1 << WGM01);
		break;
		
		default:
		TCCR0A |= (1 << WGM00) | (1 << WGM01);
	}
	
	//ICR1H = 0x04;
	//ICR1L = 0XE1;
}

void initPWM0FastB(uint8_t prescaler, uint8_t mode){
	TCCR0B = 0;
	// Prescaler
	switch (prescaler){
		case 0:
		TCCR0B |= (1 << CS00);
		break;
		case 1:
		TCCR0B |= (1 << CS01);
		break;
		case 2:
		TCCR0B |= (1 << CS00) | (1 << CS01);
		break;
		case 3:
		TCCR0B |= (1 << CS02);
		break;
		case 4:
		TCCR0B |= (1 << CS00) | (1 << CS02);;
		break;
		default:
		TCCR0B |= (1 << CS00);
	}
	// Modo
	switch (mode){
		case 0:
		TCCR0B &= ~(1 << WGM02);
		break;
		case 1:
		TCCR0B |= (1 << WGM02);
		break;
		default:
		TCCR0B &= ~(1 << WGM02);
	}
}

void initPWM1FastA(uint8_t inverted, uint8_t mode){
	DDRB |= (1 << DDB1);
	DDRB |= (1 << DDB2);
	TCCR1A = 0;
	// Invertido o no
	if(inverted == 1){
		TCCR1A |=  (1 << COM1A1) | (1 << COM1A0);
		TCCR1A |=  (1 << COM1B1) | (1 << COM1B0);
	}
	else {
		TCCR1A |= (1 << COM1A1);
		TCCR1A |= (1 << COM1B1);
	}
	
	// Modo
	switch (mode){
		case 0:
			TCCR1A |= (1 << WGM10);
		break;
		case 1:
			TCCR1A |= (1 << WGM11);
		break;
		case 2:
			TCCR1A |= (1 << WGM10) | (1 << WGM11);
		break;
		case 3:
			TCCR1A |= (1 << WGM10);
		break;
		case 4:
			TCCR1A |= (1 << WGM10) | (1 << WGM11);
		break;
		
		default:
			TCCR1A |= (1 << WGM10);
	}
	
	//ICR1H = 0x04;
	//ICR1L = 0XE1;
}

void initPWM1FastB(uint8_t prescaler, uint8_t mode){
	TCCR1B = 0;
	// Prescaler
	switch (prescaler){
		case 0:
			TCCR1B |= (1 << CS10);
		break;
		case 1:
			TCCR1B |= (1 << CS11);
		break;
		case 2:
			TCCR1B |= (1 << CS10) | (1 << CS11);
		break;
		case 3:
			TCCR1B |= (1 << CS12);
		break;
		case 4:
			TCCR1B |= (1 << CS10) | (1 << CS12);
		break;
		default:
			TCCR1B |= (1 << CS10);
	}
	// Modo
	switch (mode){
		case 0:
			TCCR1B |= (1 << WGM12);
		break;
		case 1:
			TCCR1B |= (1 << WGM12);
		break;
		case 2:
			TCCR1B |= (1 << WGM12);
		break;
		case 3:
			TCCR1B |= (1 << WGM12) | (1 << WGM13);
		break;
		case 4:
			TCCR1B |= (1 << WGM12) | (1 << WGM13);
		break;
		default:
			TCCR1B |= (1 << WGM12);
	}
}

void initPWM2FastA(uint8_t inverted, uint8_t mode){
	DDRD |= (1 << DDD3);
	DDRB |= (1 << DDB3);
	TCCR2A = 0;
	// Invertido o no
	if(inverted == 1){
		TCCR2A |=  (1 << COM2A1) | (1 << COM2A0);
		TCCR2A |=  (1 << COM2B1) | (1 << COM2B0);
	}
	else {
		TCCR2A |= (1 << COM2A1);
		TCCR2A |= (1 << COM2B1);
	}
	
	// Modo
	switch (mode){
		case 0:
		TCCR2A |= (1 << WGM20) | (1<< WGM21);
		break;
		case 1:
		TCCR2A |= (1 << WGM20) | (1 << WGM21);
		break;
		
		default:
		TCCR2A |= (1 << WGM20) | (1 << WGM21);
	}
	
	//ICR1H = 0x04;
	//ICR1L = 0XE1;
}

void initPWM2FastB(uint8_t prescaler, uint8_t mode){
	TCCR2B = 0;
	// Prescaler
	switch (prescaler){
		case 0:
		TCCR2B |= (1 << CS20);
		break;
		case 1:
		TCCR2B |= (1 << CS21);
		break;
		case 2:
		TCCR2B |= (1 << CS22);
		break;
		case 3:
		TCCR2B |= (1 << CS22) | (1 << CS21);
		break;
		case 4:
		TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22);
		break;
		default:
		TCCR2B |= (1 << CS20);
	}
	// Modo
	switch (mode){
		case 0:
		TCCR2B &= ~(1 << WGM22);
		break;
		case 1:
		TCCR2B |= (1 << WGM22);
		break;
		default:
		TCCR2B &= ~(1 << WGM22);
	}
}

void updateDutyCycle1A(uint8_t Cycle){
	OCR1A = Cycle;
}

void updateDutyCycle1B(uint8_t Cycle){
	OCR1B = Cycle;
}

void updateDutyCycle2A(uint8_t Cycle){
	OCR2A = Cycle;
}

void updateDutyCycle2B(uint8_t Cycle){
	OCR2B = Cycle;
}

void updateDutyCycle0A(uint8_t Cycle){
	OCR0A = Cycle;
}

void updateDutyCycle0B(uint8_t Cycle){
	OCR0B = Cycle;
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

void initDisplay(void){
	// Puerto D como salida
	DDRD = 0xFF;
	PORTD = 0;
	// Utilizar Rx y Tx
	UCSR0B = 0;
}

void Nums(uint8_t Numero){
	switch(Numero){
		case 0:
			PORTD = 0x3F;
			break;
		case 1:
			PORTD = 0x06;
			break;
		case 2:
			PORTD = 0x5B;
			break;
		case 3:
			PORTD = 0x4F;
			break;
		case 4:
			PORTD = 0x66;
			break;
		case 5:
			PORTD = 0x6D;
			break;
		case 6:
			PORTD = 0x7D;
			break;
		case 7:
			PORTD = 0x07;
			break;
		case 8:
			PORTD = 0x7F;
			break;
		case 9:
			PORTD = 0x6F;
			break;
		default:
			PORTD = 0x00;
	}
}

// Enviar un comando a la LCD
void lcd_command(char a) {
	PORTB &= ~(1<<PB3);
	lcd_port(a);
	PORTB |= (1<<PB2);
	_delay_ms(4);
	PORTB &= ~(1<<PB2);
}

void lcd_port(char a){
	if (a & 1)
		PORTD |= (1<<PORTD2);
	else
		PORTD &= ~(1<<PORTD2);
	
	if (a & 2)
		PORTD |= (1<<PORTD3);
	else
		PORTD &= ~(1<<PORTD3);
	
	if (a & 4)
		PORTD |= (1<<PORTD4);
	else
		PORTD &= ~(1<<PORTD4);
	
	if (a & 8)
		PORTD |= (1<<PORTD5);
	else
		PORTD &= ~(1<<PORTD5);
	
	if (a & 16)
		PORTD |= (1<<PORTD6);
	else
		PORTD &= ~(1<<PORTD6);
	
	if (a & 32)
		PORTD |= (1<<PORTD7);
	else
		PORTD &= ~(1<<PORTD7);
	
	if (a & 64)
		PORTB |= (1<<PORTB0);
	else
		PORTB &= ~(1<<PORTB0);
	
	if (a & 128)
		PORTB |= (1<<PORTB1);
	else
		PORTB &= ~(1<<PORTB1);	
}

// Enviar un carácter a la LCD
void lcd_write_char(char data) {
	PORTB |= (1<<PORTB3);
	lcd_port(data);
	PORTB |= (1<<PORTB2);
	_delay_ms(4);
	PORTB &= ~(1<<PORTB2);
	
}

// Inicializar LCD en modo 8 bits
void lcd_init() {
	DDRD = 0xFF; // Configurar pines de PORTD como salida
	DDRB = 0x0F; // Configurar pines de PORTB como salida
	PORTD = 0;
	PORTB = 0;
	
	/*
	lcd_port(0x00);
	_delay_ms(15);
	lcd_command(0x30);
	_delay_ms(5);
	lcd_command(0x30);
	_delay_ms(5);
	lcd_command(0x30);
	_delay_ms(10);
	*/
	
	lcd_command(0x38);
	_delay_us(100);
	lcd_command(0x0C);
	_delay_us(100);
	lcd_command(0x01);
	_delay_us(100);
	lcd_command(0x06);
	_delay_us(100);
}

// Posicionar cursor en fila/columna
void lcd_setCursor(char c, char f) {
	char temp;
	
	if (f == 1){
		temp = 0x80 + c - 1;
		lcd_command(temp);
		} else if (f == 2){
		temp = 0xC0 + c - 1;
		lcd_command(temp);
	}
}

// Escribir una cadena completa
void lcd_write_string(char *str) {
	while (*str) {
		lcd_write_char(*str++);
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

void floatToStringNo(float num, char *str, int precision) {
	int intPart = (int)num;  // Parte entera
	//float decimalPart = num - intPart;  // Parte decimal

	// Convertir parte entera con itoa()
	itoa(intPart, str, 10);

	// Agregar punto decimal
	int i = 0;
	while (str[i] != '\0') i++;  // Buscar el final de la cadena

	str[i] = '\0';  // Terminar la cadena
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

static void spiReceiveWait(){
	while(!(SPSR & (1<<SPIF)));
}

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

//I2C
//iniciar maestro
void InitMasterI2C(unsigned long SCL_CLOCK, uint8_t Prescaler){
	DDRC &= ~((1<<DDC4)|(1<<DDC5)); //pines i2c entrada
	
	switch(Prescaler){
		case 1:
			TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
		break;
		case 4:
			TWSR &= ~(1<<TWPS1);
			TWSR |= (1<<TWPS0);
		break;
		case 16:
			TWSR &= ~(1<<TWPS0);
			TWSR |= (1<<TWPS1);
		break;
		case 64:
			TWSR |= ((1<<TWPS1)|(1<<TWPS0));
		break;
		default:
			TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
			Prescaler = 1;
		break;
	}
	TWBR = ((F_CPU/SCL_CLOCK)-16)/(2*Prescaler);
	TWCR |= (1<<TWEN);
}
//inicio com
void StartMasterI2C(void){
	//uint8_t estado;
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //ini cond start
	while(!(TWCR & (1<<TWINT))); //esperar termina flag TWINT
}
//parar com
void StopMasterI2C(void){
	TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO); //ini sec STOP
}
//transm datos, devuelve 0 si slave recibio
uint8_t WriteMasterI2C(uint8_t dato){
	uint8_t estado;
	
	TWDR = dato;
	TWCR = (1<<TWEN)|(1<<TWINT); //ini envio
	
	while(!(TWCR & (1<<TWINT))); //espera flag TWINT
	estado = TWSR & 0xF8;
	// verificar si fue SLA + W con ACK (direccion con ganas de escribir)
	// SLA + R con ACK (direccion con escritura), o un Dato con ACK
	if (estado == 0x18 || estado == 0x28 || estado == 0x40)
	{
		return 1;
	}
	else{
		return estado;
	}
}
//recibir datos por slave a master
uint8_t ReadMasterI2C(uint8_t *buffer, uint8_t ack){
	uint8_t estado;
	if (ack)
	{
		TWCR |= (1<<TWEA); //lectura con ACK
	}
	else
	{
		TWCR &= ~(1<<TWEA); //lectura sin ACK
	}
	
	TWCR |= (1<<TWINT); //ini lect
	while(!(TWCR & (1<<TWINT))); //espera TWINT
	estado = TWSR & 0xF8;
	if (estado == 0x58 || estado == 0x50)
	{
		*buffer = TWDR;
		return 1;
	}
	else{
		return estado;
	}
}
//iniciar slave
void InitSlaveI2C(uint8_t address){
	DDRC &= ~((1<<DDC4)|(1<<DDC5)); //pines i2c como entradas
	TWAR = address << 1; //asigna direccion
	//habilita la interfaz, ACK automatico, habilita ISR
	TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWIE); 
}