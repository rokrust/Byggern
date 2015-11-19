#ifndef _MCP_H
#define _MCP_H
#include <stdint.h>

char mcp_read(uint8_t address);
void mcp_write(uint8_t address, char data);
void mcp_reset();
void mcp_bitModify(uint8_t address, uint8_t mask, char data);
void mcp_requestToSend(uint8_t buffer);
uint8_t mcp_readStatus();
void mcp_init(void);
#endif