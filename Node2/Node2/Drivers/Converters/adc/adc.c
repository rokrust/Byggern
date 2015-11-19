#include <avr/io.h>
#include <stdint.h>
void adc_init(void){
	ADCSRA	|= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADMUX	|= (1<<REFS0);
	ADMUX	&= ~(1<<REFS1);
}

uint16_t adc_read(uint8_t channel){
	ADMUX	|= ((1<<channel) & 0x03);
	ADCSRA	|= (1 << ADSC);

	while((ADCSRA & (1<<ADSC))){ ; }

	return ADC;
}