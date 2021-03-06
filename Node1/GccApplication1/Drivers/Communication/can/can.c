#include "can.h"
#include "mcp/mcp.h"
#include "mcp/mcp_defines.h"
#include "../../MultifunctionCard/oled/oled.h"

#include <stdlib.h>
#include <avr/io.h>

void can_init(void){
	mcp_init();
	mcp_bitModify(MCP_RXB0CTRL, MCP_RXBCTRL_MASK, 0xff);
	mcp_bitModify(MCP_RXB1CTRL, MCP_RXBCTRL_MASK, 0xff);	
	
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
	if(PIND | (1 << PB3)){ //Check for interrupts
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
	
	return 0x00;
	
}

void can_print(can_message msg){
	for(int i = 0; i < msg.length; i++){
		printf("%c", msg.data[i]);
	}
	printf("\n");
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

void can_send_joystick_message(){
	joy_read_joystick();
	Joystick joy = joy_get_joystick();
	uint8_t joy_data = joy.x;
	oled_set_write_position(0,0);
	oled_printf("%d\n", joy.x);
	//oled_printf("Test");
	
	can_message msg = new_can_message(MCP_JOYSTICK_MESSAGE, 1, &joy_data);
	can_write(&msg, MCP_TXB0CTRL);
}

void can_send_slider_message(){
	uint8_t msg_data = joy_read_left_slider();
	can_message msg = new_can_message(MCP_SLIDER_MESSAGE, 1, &msg_data);
	can_write(&msg, MCP_TXB0CTRL);
}
void can_send_button_message(){
	uint8_t msg_data= joy_read_right_button();
	can_message msg = new_can_message(MCP_SOLENOID_MESSAGE, 1, &msg_data);
	can_write(&msg, MCP_TXB0CTRL);
}
void can_send_pid_message(uint8_t kp_coeff, uint8_t kp_exp, uint8_t ki_coeff, uint8_t ki_exp, uint8_t kd_coeff, uint8_t kd_exp){
	//Using the scientific form of the numbers.
	uint8_t parameters[6] = {kp_coeff, kp_exp, ki_coeff, ki_exp, kd_coeff, kd_exp}; 
	can_message msg = new_can_message(MCP_PID_MESSAGE, 6, parameters);
	can_write(&msg, MCP_TXB0CTRL);
}