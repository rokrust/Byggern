#include "../../TWI/twi_master.h"
#include "../motor/motor_driver.h"
#include <avr/interrupt.h>
#include <stdint.h>

#define MAX520_TWI_ADDR_BASE 0b01010000
static uint8_t twi_address = 0x00;

void dac_initialize(uint8_t MAX520_addr){
	TWI_Master_Initialise();
	sei();
	
	twi_address = MAX520_addr;
}	


void dac_write(uint8_t channel, uint8_t val){
	uint8_t message[3] = {	MAX520_TWI_ADDR_BASE | ((twi_address & 0x07) << 1), //Set address
							channel & 0x07, //Channel no higher than 0x07
							val		};
							
	TWI_Start_Transceiver_With_Data(message, 3);
}