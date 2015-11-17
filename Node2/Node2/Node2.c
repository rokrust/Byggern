/*
 * Node2.c
 *
 * Created: 23.10.2015 14:22:15
 *  Author: haakome
 */ 

#include "Config/config.h"
#include "Drivers/pwm/pwm.h"
#include "Drivers/usart/usart.h"
#include "Drivers/spi/spi.h"
#include "Drivers/mcp/mcp.h"
#include "Drivers/mcp/mcp_defines.h"
#include "Drivers//can/can.h"
#include "Drivers/adc/adc.h"
#include "Drivers/motor/motor_driver.h"
#include "Drivers/dac/dac.h"
#include "Controller/pid.h"

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
	pid_init(-1.0, -2.0, -0.0);
	
	can_message msg = {0};
	uint16_t max_encoder_value = pid_encoder_max_value();
	
	while(1)
	{
		if(can_pollInterrupt()){
			msg = can_read();
		}
		printf("%d\n", adc_read(0));
		can_transmit_ir_value(adc_read(0));
		can_handle_message(msg, max_encoder_value);
		
		_delay_ms(10);
	}
	
	return 1;
}