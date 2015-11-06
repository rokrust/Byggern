#include <stdint.h>
#include "joy.h"

typedef struct can_message can_message;

struct can_message{
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
	
};

void can_init(void);

can_message new_can_message(uint16_t id, uint8_t length, uint8_t* data);
can_message can_read(void);

//tx_buffer is the address of the control register
void can_write(can_message *msg, uint8_t tx_buffer);

uint8_t can_pollInterrupt(void);

void can_print(can_message msg);
void can_send_joystick_message(Joystick joy_position);