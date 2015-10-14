#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

void SPI_masterInit(void){
	/* Set MOSI and SCK output, all others input */
	DDRB = (1 << PB5)|(1 << PB7)|(1 << PB4); //4 = SS, 5 = MOSI, 7 = SCK
	DDRB &= ~(1 << PB6); //DDRB6
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
void SPI_transmit(char cData){
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}