#include "motor_driver.h"
#include "dac.h"
#include "motor_controller.h"

#include <avr/io.h>
#include <stdint.h>
#include <avr/delay.h>

//Reference position is a number between encoder min and encoder max.
float p_controller(float reference_position, float max_encoder_value){
	//Convert slider to encoder values
	//printf("Ref: %d\n", reference_position);
	//New speed of the controller
	int16_t error = reference_position-motor_encoder_read();
	//printf("Enc: %d\n", motor_encoder_read());
	//printf("Error: %d\n", error);
	return ((k_p*error)*255)/max_encoder_value;
}

uint16_t pi_controller(uint16_t reference_position, uint16_t *error, uint16_t max_encoder_value){
	//Convert slider to encoder values
	reference_position *= (max_encoder_value/OVERFLOW_FIX)/255 * OVERFLOW_FIX;
	uint16_t current_position = motor_encoder_read();
	*error += reference_position-current_position;
	
	//New speed of the controller
	return -((float)(k_p*(reference_position-current_position) + k_i*(*error))*255)/max_encoder_value;
}

uint16_t controller_init(){
	motor_speed(75);
	uint16_t enc_val = 0, prev_enc_val = -1;
	while(enc_val != prev_enc_val){
		enc_val = motor_encoder_read();
		_delay_ms(100);
		prev_enc_val = motor_encoder_read();
	}
	motor_speed(0);
	motor_encoder_reset();
	
	motor_speed(-75);
	enc_val = 0, prev_enc_val = -1;
	while(enc_val != prev_enc_val){
		enc_val = motor_encoder_read();
		_delay_ms(100);
		prev_enc_val = motor_encoder_read();
	}
	motor_speed(0);
	
	//Find max value of encoder.
	return enc_val;
}