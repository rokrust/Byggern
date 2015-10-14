#include "mcp.h"
#include "mcp_defines.h"
#include "spi.h"

char mcp_read(uint8_t address){
	PORTB &= ~(1 << MCP_CS);
	SPI_transmit(MCP_READ);
	SPI_transmit(address);
	
	char data = SPI_transmit(0x00);
	PORTB |= (1 << MCP_CS);
	
	return data;
}
void mcp_write(uint8_t address, char data){
	PORTB &= ~(1 << MCP_CS);
	
	SPI_transmit(MCP_WRITE);
	SPI_transmit(address);
	SPI_transmit(data);
	
	PORTB |= (1<< MCP_CS);
}	
void mcp_reset(){
	PORTB &= ~(1 << MCP_CS);
	
	SPI_transmit(MCP_RESET);
	
	PORTB |= (1<< MCP_CS);
}	
void mcp_bitModify(uint8_t address, uint8_t mask, char data){
	PORTB &= ~(1 << MCP_CS);
	
	SPI_transmit(MCP_BIT_MODIFY);
	SPI_transmit(address);
	SPI_transmit(mask);
	SPI_transmit(data);
	
	PORTB |= (1<< MCP_CS);
}
void mcp_requestToSend(uint8_t buffer){
	PORTB &= ~(1 << MCP_CS);
	SPI_transmit(buffer);
	
	PORTB |= (1<< MCP_CS);
} 
uint8_t mcp_read_status(void){
	PORTB &= ~(1 << MCP_CS);
	SPI_transmit(MCP_READ_STATUS);
	uint8_t status = SPI_transmit(0x00);
	PORTB |= (1<< MCP_CS);
	
	return status;
}