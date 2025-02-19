 /* labo4.c
 *
 * Created: 2/13/2025 4:21:19 PM
 * Author : cssos
 */

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>

volatile uint8_t contador = 0;  // 4 bits (0-15)
uint8_t buffer = 0;

void leds(void);
void initPorts(void);
#include "I2C_SLAVE.h"

#define slave_address 0x40

int main(void)
{
	cli();
	initPorts();
	leds();
	I2C_slave_init(slave_address);
	sei();

	while (1)
	{
		// Verifica si se presiona el botón de incrementar
		if (!(PINC & (1 << PORTC0))) {
			_delay_ms(50); // Antirrebote
			while (!(PINC & (1 << PORTC0))); // Esperar a que se suelte
			if (contador < 15) contador++;  // Limitar a 4 bits
			leds();
		}

		// Verifica si se presiona el botón de decrementar
		if (!(PINC & (1 << PORTC1))) {
			_delay_ms(50); // Antirrebote
			while (!(PINC & (1 << PORTC1))); // Esperar a que se suelte
			if (contador > 0) contador--;  // Evitar negativos
			leds();
		}
		if (buffer == 'C'){
			PINB |= (1 << PINB5);
			buffer = 0;
		}
	}
}

void initPorts(void) {
	// Configurar pines de botones como entrada con pull-up
	DDRC &= ~((1 << PORTC0) | (1 << PORTC1)); //entradas 
	PORTC |= (1 << PORTC0) | (1 << PORTC1); //pull up
	DDRC &= ~((1 << PORTC4) | (1 << PORTC5)); //Entradas
	PORTC |= (1 << PORTC4) | (1 << PORTC5); //pull up

	// Configurar pines de LEDs (D2-D5) como salida
	DDRD |= 0b00111100; // D2-D5 como salida
	PORTD &= ~0b00111100; // Apagar inicialmente
	DDRB |= (1 << DDB5); //portb salidas
	
}

void leds(void) {
	PORTD &= ~0b00111100; // Apagar LEDs en D2-D5
	PORTD |= ((contador & 0b00001111) << 2); // Desplazar bits a D2-D5
}

ISR(TWI_vect){
	uint8_t estado;
	estado = TWSR & 0xF8;
	switch(estado){
		case 0x60:
		case 0x70:
			TWCR |= (1 << TWINT)|(1 << TWEA)|(1 << TWEN)|(1 << TWIE);
			break;
		case 0x80:
		case 0x90:
			buffer = TWDR;
			TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE); //limpiar bandera
			break;
		case 0xA8:
		case 0xB8:
			TWDR = contador; //cargar el dato
			TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE); //enviar
		break;
		default:
		TWCR |= (1 << TWINT) | (1 << TWSTO) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
		break;
		
	}
	
	
}