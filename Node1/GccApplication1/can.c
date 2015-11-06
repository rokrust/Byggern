#include "can.h"
#include "mcp.h"
#include "mcp_defines.h"

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
	
	printf("No interrupt detected!");
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

void can_send_joystick_message(Joystick joy_position){
	uint8_t joy_data[4] = {joy_position.x, joy_position.y, joy_position.offset_y, joy_position.offset_y};
	can_message msg = new_can_message(MCP_JOYSTICK_MESSAGE, 4, joy_data);
	can_write(&msg, MCP_TXB0CTRL);
}
