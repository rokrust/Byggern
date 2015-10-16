#include "can.h"
#include "mcp.h"
#include "mcp_defines.h"

#include <stdlib.h>
void can_init(){
	mcp_init();
	mcp_bitModify(RXB0CTRL, MCP_RXBCTRL_MASK, 0xff);
	mcp_bitModify(RXB1CTRL, MCP_RXBCTRL_MASK, 0xff);	
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
}

can_message can_read(void){
	can_message msg;
	uint8_t bufferSelect = can_pollInterrupt();
	
	if(!bufferSelect){
		printf("No interrupt detected!");
		return;
	}
	
	msg.id = (can_read(bufferSelect + MCP_RXSIDH_OFFSET) << 3) | (can_read(bufferSelect + MCP_RXSIDL_OFFSET) >> 5);
	msg.length = can_read(bufferSelect + MCP_RXBDLC_OFFSET) & MCP_RXBDLC_MASK;
	
	for(int i = 0; i < msg.length; i++){
		msg.data[i] = can_read(bufferSelect + MCP_RXB_OFFSET + i);
	}
	
	return msg;
}

//tx_buffer is the address of the control register
void can_write(can_message *msg, uint8_t tx_buffer){
	while(mcp_read(tx_buffer) & (1 << MCP_TEXREQ));	//wait for previous message to send
	
	mcp_write(tx_buffer + MCP_TXSIDH_OFFSET, msg->id & MCP_SIDH_MASK); //enter higher id
	mcp_write(tx_buffer + MCP_TXSIDL_OFFSET, msg->id & MCP_SIDL_MASK); //enter lower id
	
	mcp_write(tx_buffer + MCP_TXBDLC_OFFSET, msg->length & MCP_BDLC_MASK); //enter length of message
	
	for(int i = 0; i < msg->length; i++){
		mcp_write(tx_buffer + MCP_TXB_OFFSET + i, msg->data[i]);
	}
	
	mcp_requestToSend(0x80 + (tx_buffer >> 4) - 0x02); //Convert TXBnCTRL to RTS-code
	
}

uint8_t can_pollInterrupt(void){
	if(PIND | (1 << PB3)){ //Check for interrupts
		uint8_t interrupt_register = mcp_read(MCP_CANINTF);
		//Finds the register that sent an interrupt signal
		if(interrupt_register & MCP_RX0IF){
			mcp_bitModify(MCP_CANINTF, MCP_RX0IF, 0xFF);
			return RXB0CTRL;
		}
		
		else if(interrupt_register & MCP_RX1IF){
			mcp_bitModify(MCP_CANINTF, MCP_RX1IF, 0xFF);
			return RXB1CTRL;
		}	
	}
	
	else{
		printf("No interrupt detected!");
		return 0x00;
	}
}

void can_print(can_message msg){
	for(int i = 0; i < msg.length; i++){
		printf("%d", msg.data[i]);
	}
}
	