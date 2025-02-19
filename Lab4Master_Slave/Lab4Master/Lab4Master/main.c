/*
 * Lab4Master.c
 *
 * Created: 13/02/2025 04:40:25 p. m.
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

#define slave1 0x40
#define slave2 0x30

uint8_t direccion;
uint8_t contador;
uint8_t ValCont = 0;
uint8_t pote;
uint8_t ValorPote;
float ConvPote;
uint8_t bufferI2C;
uint8_t ValorI2C = 0;

char buf[20];
char buff_I2C[3] = {0};

void Setup(void);

int main(void)
{
	PORTD = 0;
	PORTB = 0;
	Setup();
    lcd_setCursor(1,1);
    lcd_write_string("   S1:    S2:");
	/*ValCont = 8;
	itoa(ValCont, buf, 10);
	lcd_setCursor(4,2);
	lcd_write_string(buf);*/
    while (1) 
    {
		//Escritura
		StartMasterI2C();
		bufferI2C = slave1 << 1 & 0b11111110;
		
		contador = WriteMasterI2C(bufferI2C);
		if (contador != 1)
		{
			lcd_setCursor(1,1);
			itoa(contador, buf, 10);
			lcd_write_string(buf);
			//lcd_write_string("NO");
			StopMasterI2C();
		}
		else if (contador == 1)
		{
			WriteMasterI2C('C');
			lcd_setCursor(1,1);
			lcd_write_string("SI");
			StopMasterI2C();
		}
		_delay_ms(100);
		//Lectura
		StartMasterI2C();
		bufferI2C = slave1 << 1 | 0b00000001;
		
		contador = WriteMasterI2C(bufferI2C);
		if (contador != 1)
		{
			StopMasterI2C();
			lcd_setCursor(1,2);
			itoa(contador, buf, 10);
			lcd_write_string(buf);
			//lcd_write_string("NO");
		}
		else if (contador == 1)
		{
			lcd_setCursor(1,2);
			lcd_write_string("SI");
			TWCR |= (1<<TWINT); //ini lect
			while(!(TWCR & (1<<TWINT))); //espera TWINT
			
			ValCont = TWDR;
			//ValCont = 4;
			StopMasterI2C();
			itoa(ValCont, buf, 10);
			if (ValCont < 10)
			{
				lcd_setCursor(4,2);
				lcd_write_string("0");
				lcd_setCursor(5,2);
				lcd_write_string(buf);
			}
			else if (ValCont >= 10)
			{
				lcd_setCursor(4,2);
				lcd_write_string(buf);
			}
		}
		//_delay_ms(500);
		
		StartMasterI2C();
		bufferI2C = slave2 << 1 & 0b11111110;
		
		pote = WriteMasterI2C(bufferI2C);
		if (pote != 1)
		{
			lcd_setCursor(8,1);
			itoa(pote, buf, 10);
			lcd_write_string(buf);
			//lcd_write_string("NO");
			StopMasterI2C();
		}
		else if (pote == 1)
		{
			WriteMasterI2C('P');
			lcd_setCursor(8,1);
			lcd_write_string("SI");
			StopMasterI2C();
		}
		_delay_ms(100);
		//Lectura
		StartMasterI2C();
		bufferI2C = slave2 << 1 | 0b00000001;
		
		pote = WriteMasterI2C(bufferI2C);
		if (pote != 1)
		{
			StopMasterI2C();
			lcd_setCursor(8,2);
			itoa(pote, buf, 10);
			lcd_write_string(buf);
			//lcd_write_string("NO");
		}
		else if (pote == 1)
		{
			lcd_setCursor(8,2);
			lcd_write_string("SI");
			TWCR |= (1<<TWINT); //ini lect
			while(!(TWCR & (1<<TWINT))); //espera TWINT
			
			ValorPote = TWDR;
			/*TextUART("Pote\n");
			buff_I2C[0] = ValorPote;
			TextUART(buff_I2C);*/
			ConvPote = ValorPote * 5.0/255;
			StopMasterI2C();
			floatToString(ConvPote, buf, 2);
			lcd_setCursor(10,2);
			lcd_write_string(buf);
			lcd_setCursor(14,2);
			lcd_write_string("V");
		}
		//_delay_ms(500);
    }
}

void Setup(void){
	lcd_init();
	InitMasterI2C(100000, 1);
	initUART();
}

