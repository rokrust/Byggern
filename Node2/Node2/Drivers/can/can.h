#include <stdint.h>

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
void can_write(can_message msg, uint8_t tx_buffer);

uint8_t can_pollInterrupt(void);

void can_print(can_message msg);

void can_handle_joystick_message(can_message msg);
void can_handle_solenoid_message(can_message msg);
void can_handle_slider_message(can_message msg, uint16_t max_encoder_value);
void can_handle_message(can_message msg, uint16_t max_encoder_value);