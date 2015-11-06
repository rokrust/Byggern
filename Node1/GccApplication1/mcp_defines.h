#define MCP_CS 4

#define MCP_READ 0x03
#define MCP_WRITE 0x02
#define MCP_RESET 0xC0
#define MCP_BIT_MODIFY 0x05
#define MCP_READ_STATUS 0xA0

#define MCP_TEXREQ 0x03

#define MCP_SIDH_MASK 0xFF
#define MCP_SIDL_MASK 0xE0
#define MCP_BDLC_MASK 0x0f
#define MCP_RXBCTRL_MASK 0x60
#define MCP_CANCTRL_MASK 0xE0

#define MCP_RXBDLC_MASK 0x0F

#define MCP_TXSIDH_OFFSET 0x01
#define MCP_TXSIDL_OFFSET 0x02
#define MCP_TXBDLC_OFFSET 0x05
#define MCP_TXB_OFFSET 0x06

#define MCP_RXSIDH_OFFSET 0x01
#define MCP_RXSIDL_OFFSET 0x02
#define MCP_RXBDLC_OFFSET 0x05
#define MCP_RXB_OFFSET 0x06

#define request_to_send0 0x81
#define request_to_send1 0x82
#define request_to_send2 0x84

#define MCP_RX0IF 0x01
#define MCP_RX1IF 0x02

#define MCP_TXB0CTRL 0x30
#define MCP_TXB1CTRL 0x40
#define MCP_TXB2CTRL 0x50

#define MCP_RXB0CTRL 0x60
#define MCP_RXB1CTRL 0x70

#define MCP_CANINTF 0x2C


#define MCP_CANCTRL 0x0f

#define MCP_NORMAL_MODE (0 << 5)
#define MCP_LOOPBACK_MODE (2 << 5)

#define MCP_JOYSTICK_MESSAGE 0x0055
#define MCP_GAME_SCORE_MESSAGE 0x0066
#define MCP_BUTTON_PRESS 0x0077