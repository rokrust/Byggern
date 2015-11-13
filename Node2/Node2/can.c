#include "can.h"
#include "mcp.h"
#include "mcp_defines.h"
#include "pwm.h"
#include "motor_driver.h"
#include "pid.h"

#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>
#include <math.h>

void can_init(){
	mcp_init();
	mcp_bitModify(MCP_RXB0CTRL, MCP_RXBCTRL_MASK, 0xff);
	mcp_bitModify(MCP_RXB1CTRL, MCP_RXBCTRL_MASK, 0xff);
	mcp_bitModify(MCP_CANINTE, 0x3, 0x3);
	
	mcp_bitModify(MCP_CANCTRL, MCP_CANCTRL_MASK, MCP_NORMAL_MODE);
	
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
}

can_message can_read(void){
	can_message msg;
	uint8_t bufferSelect = can_pollInterrupt();
	/*
	if(!bufferSelect){
		printf("No interrupt detected!");
	}
	*/
	msg.id = (mcp_read(bufferSelect + MCP_RXSIDH_OFFSET) << 3) | (mcp_read(bufferSelect + MCP_RXSIDL_OFFSET) >> 5);
	msg.length = mcp_read(bufferSelect + MCP_RXBDLC_OFFSET) & MCP_RXBDLC_MASK;
	
	for(int i = 0; i < msg.length; i++){
		msg.data[i] = mcp_read(bufferSelect + MCP_RXB_OFFSET + i);
	}
	
	mcp_bitModify(MCP_CANINTF, MCP_RX0IF, 0);
	return msg;
}

can_message new_can_message(uint16_t id, uint8_t length, uint8_t* data){
	can_message m;
	m.id = id;
	m.length = length;
	for(int i = 0; i < length; i++){
		m.data[i] = data[i];
	}
	
	
	return m;
}

//tx_buffer is the address of the control register
void can_write(can_message *msg, uint8_t tx_buffer){
	while(mcp_read(tx_buffer) & (1 << MCP_TEXREQ));	//wait for previous message to send
	
	mcp_write(tx_buffer + MCP_TXSIDH_OFFSET, (msg->id >> 3) & MCP_SIDH_MASK); //enter higher id
	mcp_write(tx_buffer + MCP_TXSIDL_OFFSET, (msg->id << 5) & MCP_SIDL_MASK); //enter lower id
	
	mcp_write(tx_buffer + MCP_TXBDLC_OFFSET, msg->length & MCP_BDLC_MASK); //enter length of message
	
	for(int i = 0; i < msg->length; i++){
		mcp_write(tx_buffer + MCP_TXB_OFFSET + i, msg->data[i]);
	}
	
	mcp_requestToSend(0x80 + (tx_buffer >> 4) - 0x02); //Convert TXBnCTRL address to RTS-code
	
}
uint8_t can_pollInterrupt(void){
	if(PINA | (1 << PA0)){ //Check for interrupts
		uint8_t interrupt_register = mcp_read(MCP_CANINTF);
		//Finds the register that sent an interrupt signal
		if(interrupt_register & MCP_RX0IF){
			mcp_bitModify(MCP_CANINTF, MCP_RX0IF, 0xFF);
			return MCP_RXB0CTRL;
		}
		
		else if(interrupt_register & MCP_RX1IF){
			mcp_bitModify(MCP_CANINTF, MCP_RX1IF, 0xFF);
			return MCP_RXB1CTRL;
		}	
	}
	
	//printf("No interrupt detected!");
	return 0x00;
}

void can_print(can_message msg){
	for(int i = 0; i < msg.length; i++){
		printf("%c", msg.data[i]);
	}
	printf("\n");
}

void can_handle_joystick_message(can_message msg){
	if(msg.id == MCP_JOYSTICK_MESSAGE){
		pwm_set_servo(msg.data[0]);
	}
}
void can_handle_score_message(can_message msg){
	if(msg.id == MCP_BUTTON_PRESS){
		if(msg.data[0]){
			PORTA |= (1<<PA2); //stop relay
		}
		else{
			PORTA &= ~(1<<PA2); //activate relay
		}
	}
}

extern float max_encoder_value;

void can_handle_slider_message(can_message msg){
	if(msg.id == MCP_SLIDER_MESSAGE){
		printf("In can: %d\n", msg.data[0]);
		uint8_t slider_value = msg.data[0];
		float r = (255 - slider_value)*(max_encoder_value/255.0);
		//r = max_encoder_value / 2;
		float y = motor_encoder_read();
		float u = pid_generate(r, y, 0.01) / ((max_encoder_value+1)/255);
				
		//printf("r: %.3f   ", r);
		//printf("y: %.3f   ", y);
		//printf("u: %.3f\n", u);
				
		motor_speed(u);
	}
}
		/*int16_t speed = msg.data[0];
		//printf("Speed: %d\n", speed);
		printf("Speed: %d\n", speed);
		speed -= 127;
		
		if(speed > 0){
			//printf("Speed: %d\n", speed);
			motor_direction(right);
			motor_speed(speed);
		}
		else{
			//printf("Speed: %d\n", speed);
			motor_direction(left);
			motor_speed(abs(speed));
		}*/
