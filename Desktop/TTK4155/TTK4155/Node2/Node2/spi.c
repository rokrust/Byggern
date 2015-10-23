#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

void SPI_masterInit(void){
	/* Set MOSI and SCK output, all others input */
	DDRB = (1 << PB2)|(1 << PB1)|(1 << PB0); //0 = SS, 3 = SI, 2 = SCK
	DDRB &= ~(1 << PB3); //SO
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
uint8_t SPI_transmit(char cData){
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}