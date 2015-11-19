/*
 * Node2.c
 *
 * Created: 23.10.2015 14:22:15
 *  Author: haakome
 */ 

#include "Config/config.h"
#include "Drivers/pwm/pwm.h"
#include "Drivers/Communication/usart/usart.h"
#include "Drivers/Communication/spi/spi.h"
#include "Drivers/Communication/can/mcp/mcp.h"
#include "Drivers/Communication/can/mcp/mcp_defines.h"
#include "Drivers/Communication/can/can.h"
#include "Drivers/Converters/adc/adc.h"
#include "Drivers/motor/motor_driver.h"
#include "Drivers/Converters/dac/dac.h"
#include "Drivers/Sensors/ir.h"
#include "Controller/pid.h"
#include "Solenoid/solenoid.h"

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

int main(void)
{
	sei();
	USART_Init(MYUBRR);
	fdevopen((int (*)(char,  struct __file *))USART_Transmit, (int (*)(struct __file *))USART_Receive);
	printf("UART setup done\n");
	can_init();
	printf("CAN setup done\n");
	pwm_init();
	printf("PWM setup done\n");
	adc_init();
	printf("ADC setup done\n");
	motor_init();
	printf("Motor setup done\n");
	dac_initialize(0b111);
	printf("DAC setup done\n");
	solenoid_init();
	printf("Solenoid setup done\n");
	printf("Initialization done\n");
	ir_init();
	solenoid_init();
	pid_init(-0.7, -1.5, -0.0);
	DDRF |= ~(1 << PF0);
	can_message msg = {0};
	uint16_t max_encoder_value = pid_encoder_max_value();
	uint8_t bufferSelect;
	while(1)
	{
		bufferSelect = can_pollInterrupt();
		if(bufferSelect){
			msg = can_read(bufferSelect);
		}
		can_handle_message(msg, max_encoder_value);
		printf("%d\n", adc_read(0));
		ir_beam_blocked(adc_read(0));
		_delay_ms(10);
	}
	
	return 1;
}